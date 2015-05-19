/* 
 * File:   GUI_Console.h
 * Author: adayoldbagel
 *
 * Created on May 19, 2015, 1:47 PM
 */

#ifndef GUI_CONSOLE_H
#define	GUI_CONSOLE_H

#include <sstream>
#include "../../Headers/Settings.h"
#include "../../Headers/loadShaders.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace DualityEngine {
    class GUI_Console {
        
        GLuint buffers[3];
        GLuint VAOloc;
        GLuint shdrLoc;
        GLuint texture;
        GLuint txtrLoc;
        GLuint attrLoc_verts;
        GLuint attrLoc_uvCoo;
        
        int charWidth;
        int charHeight;
        
        DUA_FLOAT* bodyVerts;
        DUA_FLOAT* bodyUVs;
        DUA_UINT8* bodyIndices;
        int bodyWidth;
        int bodyHeight;
        
        DUA_FLOAT* commandVerts;
        DUA_FLOAT* commandUVs;
        DUA_UINT8* commandIndices;
        int commandWidth;
        const int commandHeight = 1;
        
        #define BKGD_DEPTH DUA_ZPLANENEAR + 0.001f
        const DUA_FLOAT backgroundVerts[18] = {
            0.0, 1.0, BKGD_DEPTH,
            1.0, 1.0, BKGD_DEPTH,
            0.0, 0.45, BKGD_DEPTH,
            1.0, 0.45, BKGD_DEPTH,
            0.0, 0.5, BKGD_DEPTH,
            1.0, 0.5, BKGD_DEPTH
        };
        
        DUA_FLOAT* allVerts;
        DUA_FLOAT* allUVs;
        DUA_FLOAT* allIndices;
        int numVerts;
        int numIndices;
        
        void determineBufferData (){
            
        }
        
        void loadFontToBitmap(){
            
        }
        
    public:
        GUI_Console() {}
        ~GUI_Console() {
            glDeleteBuffers (3, buffers);
            glDeleteProgram (shdrLoc);
            glDeleteTextures (1, &texture);
            glDeleteVertexArrays (1, &VAOloc);
        }
        
        bool Init(std::stringstream output, const char* fontFileName, int consoleWidth, int consoleHeight, int charWidth, int charHeight){
            shdrLoc = loadShaders("Assets/Shaders/GUI_Console.vert", "Assets/Shaders/GUI_Console.frag", output);

            attrLoc_verts   = glGetAttribLocation(shdrLoc, "Vertex");
            attrLoc_uvCoo   = glGetAttribLocation(shdrLoc, "UV");
            txtrLoc         = glGetUniformLocation(shdrLoc, "font_texture");
        }
    };
}

#endif	/* GUI_CONSOLE_H */

