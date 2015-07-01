#include "../Headers/loadCubeMap.h"
#define  STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace DualityEngine;
    
bool DualityEngine::loadCubeMapInitial (const char* front,
                                        const char* back,
                                        const char* top,
                                        const char* bottom,
                                        const char* left,
                                        const char* right,
                                        GLuint* tex_cube,
                                        std::stringstream& engineOut) {

    // generate cubemap texture
    glGenTextures (1, tex_cube);

    // read texture from disk and buffer to GPU
    if (!readAndBufferCubeMap(front, back, top, bottom, left, right, tex_cube, engineOut))
        return false;

    // format cube map texture
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    engineOut << "Cube map loaded successfully.\n";    
    return true;
}

bool DualityEngine::readAndBufferCubeMap(const char* front,
                                         const char* back,
                                         const char* top,
                                         const char* bottom, 
                                         const char* left,
                                         const char* right,
                                         GLuint* tex_cube,
                                         Delegate<void(const std::string&)>& outputStr){
    std::stringstream tempOut;
    bool result = readAndBufferCubeMap(front, back, top, bottom, left, right, tex_cube, tempOut);
    outputStr(tempOut.str());
    return result;
}

bool DualityEngine::readAndBufferCubeMap(const char* front,
                                         const char* back,
                                         const char* top,
                                         const char* bottom, 
                                         const char* left,
                                         const char* right,
                                         GLuint* tex_cube,
                                         std::stringstream& engineOut){

    bool success = true;
    // load each image and copy into a side of the cube-map texture
    success &= loadCubeMapSide (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front, engineOut);
    success &= loadCubeMapSide (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back, engineOut);
    success &= loadCubeMapSide (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top, engineOut);
    success &= loadCubeMapSide (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom, engineOut);
    success &= loadCubeMapSide (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left, engineOut);
    success &= loadCubeMapSide (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right, engineOut);
    return success;
}

bool DualityEngine::loadCubeMapSide (GLuint texture, GLenum side_target, const char* file_name, std::stringstream& engineOut) {

    glBindTexture (GL_TEXTURE_CUBE_MAP, texture);

    int x, y, n;
    int force_channels = 4;
    unsigned char*  image_data = stbi_load (file_name, &x, &y, &n, force_channels);

    if (!image_data) {
        engineOut << "ERROR: could not load " << file_name << std::endl;
        return false;
    }

    // non-power-of-2 dimensions check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
        engineOut << "WARNING: image " << file_name << " is not power-of-2 dimensions\n";
    }

    // copy image data into 'target' side of cube map
    glTexImage2D (side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    free (image_data);
    return true;
}
