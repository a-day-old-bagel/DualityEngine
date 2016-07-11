/*
 * File:   DebugBackground.h
 * Author: adayoldbagel
 *
 * Contains everything necessary to display a black screen with the words "NO CAM" in the center.
 */

#ifndef DEBUGBACKGROUND_H
#define DEBUGBACKGROUND_H

#include <sstream>
#include "settings.h"
#include "loadShaders.h"

namespace DualityEngine {
    
    class DebugBackground{
    private:
        // OpenGL values to store references to locations on the GPU
        GLuint VAOloc;
        GLuint vertices;
        GLuint texture;
        GLuint shdrLoc;
        GLuint attrLoc_verts;
        GLuint txtrLoc;
        bool hasInitialized;
        static const DUA_float corners[];
        static const DUA_colorByte rawTexture[];
        
    public:

        DebugBackground();
        ~DebugBackground();
        bool Init(std::stringstream& output);
        void render();
    };
}

#endif
