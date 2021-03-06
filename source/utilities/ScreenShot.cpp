//
// Created by volundr on 7/23/16.
//

#include "settings.h"
#include "ScreenShot.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define CHECK_ERROR(context) GLenum error = glGetError(); if (error != GL_NO_ERROR) { fprintf(stderr, context " 0x%04x\n", error); }

namespace DualityEngine {

    int ScreenShotTaker::init(uint32_t width, uint32_t height) {

        this->width = width;
        this->height = height;

        // Bind the screen shot frame buffer for to do color and depth buffer creation
        glGenFramebuffers(1, &fbo);

        // Create the color component of the framebuffer as a texture
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

        // Create the depth component of the framebuffer as a render buffer
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            fprintf(stderr, "Screen shot framebuffer creation failed: 0x%04x\n", status);
            return 1;
        }

        // Re-bind the default frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        CHECK_ERROR("SCREEN SHOT INIT");

        return 0;
    }

    int ScreenShotTaker::injectScreenShotState(Bank& bank) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glGetIntegerv(GL_VIEWPORT, (GLint*)&savedViewPort);
        glViewport(0, 0, width, height);
        savedCamera.bank = &bank;
        savedCamera.camId = savedCamera.bank->activeFreeCameraID;
        CameraFree* currCam = savedCamera.bank->getCameraFreePtr(savedCamera.camId);
        savedCamera.aspect = currCam->aspect;
        currCam->aspect = ( (float)width/(float)height);
        savedCamera.bank->stateOn(savedCamera.camId, RECALCPROJMAT);
        CHECK_ERROR("SCREEN SHOT STATE INJECTION");
        return 0;
    }

    int ScreenShotTaker::retrieveImage(uint8_t** pixBuf) {
        *pixBuf = new uint8_t[width * height * 4];
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, *pixBuf);
        CHECK_ERROR("SCREEN SHOT RETRIEVAL");
        return 0;
    }

    int ScreenShotTaker::retrieveImageFlipped(uint8_t** pixBuf) {
        retrieveImage(pixBuf);
        uint8_t* tempRow = new uint8_t[width * 4];
        for (uint32_t i = 0; i < height / 2; ++i) {
            memcpy(tempRow, *pixBuf + i * width * 4, width * 4);
            memcpy(*pixBuf + i * width * 4, *pixBuf + (width * (height - 1) * 4 - i * width * 4), width * 4);
            memcpy(*pixBuf + (width * (height - 1) * 4 - i * width * 4), tempRow, width * 4);
        }
        CHECK_ERROR("SCREEN SHOT FLIPPING");
        return 0;
    }

    int ScreenShotTaker::writeImageToFile(const char* filename) {
        uint8_t* pixData = NULL;
        retrieveImageFlipped(&pixData);
        stbi_write_png(filename, width, height, 4, pixData, 0);
        delete pixData;
        CHECK_ERROR("SCREEN SHOT FILE WRITING");
        return 0;
    }

    int ScreenShotTaker::revertScreenShotState() {
        CameraFree* currCam = savedCamera.bank->getCameraFreePtr(savedCamera.camId);
        currCam->aspect = savedCamera.aspect;
        savedCamera.bank->stateOn(savedCamera.camId, RECALCPROJMAT);
        glViewport(savedViewPort.x, savedViewPort.y, savedViewPort.w, savedViewPort.h);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_ERROR("SCREEN SHOT STATE REVERSAL");
        return 0;
    }
}