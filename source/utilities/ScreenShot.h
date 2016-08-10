//
// Created by volundr on 7/23/16.
//

#ifndef LASERMAPPINGDRONE_SCREENSHOT_H
#define LASERMAPPINGDRONE_SCREENSHOT_H

#include <GL/glew.h>
#include <SDL.h>
#include "Bank.h"
#include "CameraFree.h"

namespace DualityEngine {
    struct ScreenShotTaker {
        GLuint fbo, colorBuffer, depthBuffer;
        uint32_t width, height;
        struct {
            int x, y, w, h;
        } savedViewPort;
        struct {
            Bank* bank;
            DUA_id camId;
            float aspect;
        } savedCamera;
        int init(uint32_t width, uint32_t height);
        int injectScreenShotState(Bank& bank);
        int retrieveImage(uint8_t** pixBuf);
        int retrieveImageFlipped(uint8_t** pixBuf);
        int writeImageToFile(const char* fileName);
        int revertScreenShotState();
    };
}

#endif //LASERMAPPINGDRONE_SCREENSHOT_H
