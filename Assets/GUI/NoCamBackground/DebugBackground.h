#ifndef DEBUGBACKGROUND_H
#define DEBUGBACKGROUND_H

#include <sstream>
#include "../../../Headers/Settings.h"
#include "../../../Headers/loadShaders.h"

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
        bool hasInitialized = false;
        
        
       DUA_float corners[16] = {-1.0, -1.0, 0.5, 1.0,
                                 1.0, -1.0, 0.5, 1.0,
                                 1.0,  1.0, 0.5, 1.0,
                                -1.0,  1.0, 0.5, 1.0};

        // these define the colored pixels that will be used in the bitmap data below.
        #define BG0 0x00, 0x00, 0x00 // black
        #define CL7 0x40, 0x40, 0x40 // grey
        // raw bitmap data for the texture:
        const DUA_colorByte rawTexture[768] = { // 16 x 16 x 3 (width x height x [RGB])
            BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,
            BG0,BG0,BG0,CL7,BG0,BG0,BG0,CL7,BG0,CL7,CL7,CL7,CL7,BG0,BG0,BG0,
            BG0,BG0,BG0,CL7,CL7,BG0,BG0,CL7,BG0,CL7,BG0,BG0,CL7,BG0,BG0,BG0,
            BG0,BG0,BG0,CL7,BG0,CL7,BG0,CL7,BG0,CL7,BG0,BG0,CL7,BG0,BG0,BG0,
            BG0,BG0,BG0,CL7,BG0,BG0,CL7,CL7,BG0,CL7,BG0,BG0,CL7,BG0,BG0,BG0,
            BG0,BG0,BG0,CL7,BG0,BG0,BG0,CL7,BG0,CL7,CL7,CL7,CL7,BG0,BG0,BG0,
            BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,
            BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,
            BG0,CL7,CL7,CL7,BG0,BG0,CL7,CL7,BG0,BG0,CL7,BG0,BG0,BG0,CL7,BG0,
            BG0,CL7,BG0,BG0,BG0,CL7,BG0,BG0,CL7,BG0,CL7,CL7,BG0,CL7,CL7,BG0,
            BG0,CL7,BG0,BG0,BG0,CL7,BG0,BG0,CL7,BG0,CL7,BG0,CL7,BG0,CL7,BG0,
            BG0,CL7,BG0,BG0,BG0,CL7,CL7,CL7,CL7,BG0,CL7,BG0,CL7,BG0,CL7,BG0,
            BG0,CL7,BG0,BG0,BG0,CL7,BG0,BG0,CL7,BG0,CL7,BG0,BG0,BG0,CL7,BG0,
            BG0,CL7,BG0,BG0,BG0,CL7,BG0,BG0,CL7,BG0,CL7,BG0,BG0,BG0,CL7,BG0,
            BG0,CL7,CL7,CL7,BG0,CL7,BG0,BG0,CL7,BG0,CL7,BG0,BG0,BG0,CL7,BG0,
            BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,BG0,
        };
        
    public:
        
        DebugBackground() {}
        ~DebugBackground() {
            if (hasInitialized){
                glDeleteBuffers (1, &vertices);
                glDeleteProgram (shdrLoc);
                glDeleteTextures (1, &texture);
                glDeleteVertexArrays (1, &VAOloc);
            }
        }
        bool Init(std::stringstream& output) {
            
            output << "\nBeginning initialization of noCam background.\n";
            
            // shader part
            shdrLoc = loadShaders("Assets/Shaders/noCam.vert", "Assets/Shaders/noCam.frag", output);

            attrLoc_verts   = glGetAttribLocation(shdrLoc, "Vertex");
            txtrLoc = glGetUniformLocation(shdrLoc, "texPosition");
            
            // buffer part    
            glGenVertexArrays(1, &VAOloc);
            glBindVertexArray(VAOloc);

            glGenBuffers(1, &vertices);         
            
            glBindBuffer(GL_ARRAY_BUFFER, vertices);
            glBufferData(GL_ARRAY_BUFFER, sizeof(DUA_float) * 16, corners, GL_STATIC_DRAW);
            glEnableVertexAttribArray(attrLoc_verts);
            glVertexAttribPointer(attrLoc_verts, 4, GL_FLOAT, GL_FALSE, 0, 0);
            
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, rawTexture);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
            hasInitialized = true;
            output << "noCam background has initialized.\n\n";
            return true;
        }
        void render() {

            glActiveTexture (GL_TEXTURE0);
            glBindTexture (GL_TEXTURE_2D, texture);
            
            glUseProgram (shdrLoc);
            glUniform1i (txtrLoc, 0);
            glBindVertexArray (VAOloc);           
            
            glDrawArrays (GL_QUADS, 0, 4);
        }
    };
}

#endif
