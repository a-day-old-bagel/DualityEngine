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
        
//        DUA_FLOAT* bodyVerts;
//        DUA_FLOAT* bodyUVs;
//        DUA_UINT8* bodyIndices;
        int bodyWidth;
        int bodyWidthInChars;
//        float bodyW_norm;
        int bodyHeight;
        int bodyHeightInChars;
//        float bodyH_norm;
        
//        DUA_FLOAT* commandVerts;
//        DUA_FLOAT* commandUVs;
//        DUA_UINT8* commandIndices;
        int commandWidth;
        int commandWidthInChars;
//        float commandW_norm;
        int commandHeight;
        const int commandHeightInChars = 1;
//        float commandH_norm;
        
        DUA_FLOAT* allVerts;
        DUA_FLOAT* allUVs;
        DUA_UINT8* allIndices;
        int sizeVertArray;
        int sizeIndexArray;
        
        int marginWidth;
//        float marginW_norm;
        int marginHeight;
//        float marginH_norm;
        
        void determineBufferData (){
//            int numBodyVerts = (bodyWidthInChars + 1) * (bodyHeightInChars + 1);
//            int numCommVerts = (commandWidthInChars + 1) * (commandHeightInChars + 1);
            int numBodyVerts = bodyWidthInChars * bodyHeightInChars * 4;
            int numCommVerts = commandWidthInChars * commandHeightInChars * 4;
            int numBodyTris  = bodyWidthInChars * bodyHeightInChars * 2;
            int numCommTris  = commandWidthInChars * commandHeightInChars * 2;
            
//            bodyVerts = new DUA_FLOAT[numBodyVerts * 2];
//            commandVerts = new DUA_FLOAT[numCommVerts * 2];
//            DUA_FLOAT bkgdVerts[18] = {};
            
            sizeVertArray = (numBodyVerts + numCommVerts) * 2;            
            sizeIndexArray = (numBodyTris + numCommTris) * 3;
            
            allVerts = new DUA_FLOAT[sizeVertArray];
            allUVs = new DUA_FLOAT[sizeVertArray] = {0};
            allIndices = new DUA_UINT8[sizeIndexArray];
                        
            #define STRIDE(x) i * bodyWidthInChars * x + j * x
            for (int i = 0; i < bodyHeightInChars; i++){
                for (int j = 0; j < bodyWidthInChars; j++){                    
                    // Vertices of each character quad - four per quad
                    allVerts[STRIDE(8) +  0] = marginWidth  + j * charWidth;
                    allVerts[STRIDE(8) +  1] = marginHeight + i * charHeight;
                    
                    allVerts[STRIDE(8) +  2] = marginWidth  + j * charWidth  + charWidth;
                    allVerts[STRIDE(8) +  3] = marginHeight + i * charHeight;
                    
                    allVerts[STRIDE(8) +  4] = marginWidth  + j * charWidth;
                    allVerts[STRIDE(8) +  5] = marginHeight + i * charHeight + charHeight;
                    
                    allVerts[STRIDE(8) +  6] = marginWidth  + j * charWidth  + charWidth;
                    allVerts[STRIDE(8) +  7] = marginHeight + i * charHeight + charHeight;
                    
                    // UVs of each character quad - one for each vert
//                    allUVs[STRIDE(8) + 0] = 0;
//                    allUVs[STRIDE(8) + 1] = 0;
//                    
//                    allUVs[STRIDE(8) + 2] = 0;
//                    allUVs[STRIDE(8) + 3] = 0;
//                    
//                    allUVs[STRIDE(8) + 4] = 0;
//                    allUVs[STRIDE(8) + 5] = 0;
//                    
//                    allUVs[STRIDE(8) + 6] = 0;
//                    allUVs[STRIDE(8) + 7] = 0;
                    
                    // Two triangles drawn per character quad
                    allIndices[STRIDE(6) + 0] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 1] = STRIDE(4) +  3;
                    allIndices[STRIDE(6) + 2] = STRIDE(4) +  2;
                    
                    allIndices[STRIDE(6) + 3] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 4] = STRIDE(4) +  1;
                    allIndices[STRIDE(6) + 5] = STRIDE(4) +  3;
                }
            }
            
            for (int i = 0; i < commandHeightInChars; i++){
                for (int j = 0; j < commandWidthInChars; j++){
                    allVerts[i * commandWidthInChars * 8 + j * 8 +  0] = marginWidth  + j * charWidth;
                    allVerts[i * commandWidthInChars * 8 + j * 8 +  1] = marginHeight + i * charHeight;
                    
                    allVerts[i * commandWidthInChars * 8 + j * 8 +  2] = marginWidth  + j * charWidth  + charWidth;
                    allVerts[i * commandWidthInChars * 8 + j * 8 +  3] = marginHeight + i * charHeight;
                    
                    allVerts[i * commandWidthInChars * 8 + j * 8 +  4] = marginWidth  + j * charWidth;
                    allVerts[i * commandWidthInChars * 8 + j * 8 +  5] = marginHeight + i * charHeight + charHeight;
                    
                    allVerts[i * commandWidthInChars * 8 + j * 8 +  6] = marginWidth  + j * charWidth  + charWidth;
                    allVerts[i * commandWidthInChars * 8 + j * 8 +  7] = marginHeight + i * charHeight + charHeight;
                    
                    
                    allUVs[i* commandWidthInChars * 8 + j * 8 + 0] = 0;
                    allUVs[i* commandWidthInChars * 8 + j * 8 + 1] = 0;
                    
                    allUVs[i* commandWidthInChars * 8 + j * 8 + 2] = 0;
                    allUVs[i* commandWidthInChars * 8 + j * 8 + 3] = 0;
                    
                    allUVs[i* commandWidthInChars * 8 + j * 8 + 4] = 0;
                    allUVs[i* commandWidthInChars * 8 + j * 8 + 5] = 0;
                    
                    allUVs[i* commandWidthInChars * 8 + j * 8 + 6] = 0;
                    allUVs[i* commandWidthInChars * 8 + j * 8 + 7] = 0;
                }
            }
//            for (int i = 0; i < bodyHeightInChars + 1; i++){
//                for (int j = 0; j < (bodyWidthInChars + 1) * 12; j += 12){
//                    for (int k = 0; k < 4; k++){
//                        bodyVerts[i * bodyWidthInChars * 12 + j * 12 + k * 3] = marginWidth + j * charWidth;
//                        bodyVerts[i * bodyWidthInChars * 12 + j * 12 + k * 3 + 1] = marginHeight + i * charHeight;
//                        bodyVerts[i * bodyWidthInChars * 12 + j * 12 + k * 3 + 2] = DUA_ZPLANENEAR;
//                    }
//                }                
//            }
//            for (int i = 0; i < bodyHeightInChars; i++){
//                for (int j = 0; j < bodyWidthInChars; j++){
//                    //tri 1
//                    //tri 2
//                }                
//            }
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
            bodyWidth = bodyWidthInChars * charWidth;
            commandWidthInChars = bodyWidthInChars;
            commandWidth = commandWidthInChars * charWidth;
            bodyHeightInChars = consoleHeight / charHeight - commandHeightInChars;
            bodyHeight = bodyHeightInChars * charHeight;
            commandHeight = commandHeightInChars * charHeight;
            
            marginWidth = (consoleWidth % charWidth) / 2;
//            marginW_norm = (float)marginWidth / (float)consoleWidth;
            marginHeight = (consoleHeight % charHeight) / 3;
//            marginH_norm = (float)marginHeight / (float)consoleHeight;
            
            shdrLoc = loadShaders("Assets/Shaders/GUI_Console.vert", "Assets/Shaders/GUI_Console.frag", output);

            attrLoc_verts   = glGetAttribLocation(shdrLoc, "Vertex");
            attrLoc_uvCoo   = glGetAttribLocation(shdrLoc, "UV");
            txtrLoc         = glGetUniformLocation(shdrLoc, "font_texture");
            
            determineBufferData();
            loadFontToBitmap();
            
            
            // buffer part    
            glGenVertexArrays(1, &VAOloc);
            glBindVertexArray(VAOloc);

            glGenBuffers(3, buffers);

            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof allVerts, allVerts, GL_STATIC_DRAW);
            glEnableVertexAttribArray(attrLoc_verts);
            glVertexAttribPointer(attrLoc_verts, 2, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof allUVs, allUVs, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(attrLoc_uvCoo);
            glVertexAttribPointer(attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0); 

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof allIndices, allIndices, GL_STATIC_DRAW);
        }
    };
}

#endif	/* GUI_CONSOLE_H */

