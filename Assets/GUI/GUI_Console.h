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
        int bodyWidthInChars;
        int bodyHeightInChars;
        
        DUA_FLOAT* commandVerts;
        DUA_FLOAT* commandUVs;
        DUA_UINT8* commandIndices;
        int commandWidthInChars;
        const int commandHeightInChars = 1;
        
        DUA_FLOAT* allVerts;
        DUA_FLOAT* allUVs;
        DUA_UINT8* allIndices;
        int sizeVertArray;
        int sizeIndexArray;
        
        int marginWidth;
        int marginHeight;
        
        void determineBufferData (){
            int numBodyVerts = (bodyWidthInChars + 1) * (bodyHeightInChars + 1);
            int numCommVerts = (commandWidthInChars + 1) * (commandHeightInChars + 1);
            int numBodyTris  = bodyWidthInChars * bodyHeightInChars * 2;
            int numCommTris  = commandWidthInChars * commandHeightInChars * 2;
            
            bodyVerts = new DUA_FLOAT[numBodyVerts * 3];
            commandVerts = new DUA_FLOAT[numCommVerts * 3];
            DUA_FLOAT bkgdVerts[18] = {};
            
            sizeVertArray = (numBodyVerts + numCommVerts + 6) * 3;            
            sizeIndexArray = (numBodyTris + numCommTris + 4) * 3;
            
            allVerts = new DUA_FLOAT[sizeVertArray];
            allUVs = new DUA_FLOAT[sizeVertArray];
            allIndices = new DUA_UINT8[sizeIndexArray];
            
            for (int i = 0; i < numBodyVerts; i++){
                
            }
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
            
            this->charWidth = charWidth;
            this->charHeight = charHeight;
            bodyWidthInChars = consoleWidth / charWidth;
            commandWidthInChars = bodyWidthInChars;
            bodyHeightInChars = consoleHeight / charHeight - commandHeightInChars;
            marginWidth = (consoleWidth % charWidth) / 2;
            marginHeight = (consoleHeight % charHeight) / 3;
            
            shdrLoc = loadShaders("Assets/Shaders/GUI_Console.vert", "Assets/Shaders/GUI_Console.frag", output);

            attrLoc_verts   = glGetAttribLocation(shdrLoc, "Vertex");
            attrLoc_uvCoo   = glGetAttribLocation(shdrLoc, "UV");
            txtrLoc         = glGetUniformLocation(shdrLoc, "font_texture");
            
            determineBufferData();
            loadFontToBitmap();
        }
    };
}

#endif	/* GUI_CONSOLE_H */

