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
        
        GLuint buffers[2];
        GLuint texture;
        GLuint VAOloc;
        GLuint shdrLoc;
        GLuint txtrLoc;
        GLuint attrLoc_verts;
        GLuint attrLoc_uvCoo;
        
        glm::mat4 projection = glm::mat4(1.0f);
        
        int charWidth;
        int charHeight;
        
        int width;
        int widthInChars;        
        int bodyHeight;
        int bodyHeightInChars;        
        int commandHeight;
        const int commandHeightInChars = 1;
        int screenOffsetX;
        int screenOffsetY;
        
        DUA_FLOAT* allVerts;
        DUA_FLOAT* allUVs;
        DUA_UINT16* allIndices;
        int sizeVertArray;
        int sizeIndexArray;
        
        int marginWidth;
        int marginHeight;
        
        void determineBufferData (){
            int numBodyVerts = widthInChars * bodyHeightInChars * 4;
            int numCommVerts = widthInChars * commandHeightInChars * 4;
            int numBodyTris  = widthInChars * bodyHeightInChars * 2;
            int numCommTris  = widthInChars * commandHeightInChars * 2;
            
            sizeVertArray = (numBodyVerts + numCommVerts) * 2;            
            sizeIndexArray = (numBodyTris + numCommTris) * 3;
            
            allVerts = new DUA_FLOAT[sizeVertArray];
            allUVs = new DUA_FLOAT[sizeVertArray];
            allIndices = new DUA_UINT16[sizeIndexArray];
                        
            #define STRIDE(x) i * widthInChars * x + j * x

            for (int i = 0; i < bodyHeightInChars; i++){
                for (int j = 0; j < widthInChars; j++){                    
                    // Vertices of each character quad - four per quad
                    allVerts[STRIDE(8) +  0] = (2 * (marginWidth  + j * charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  1] = (2 * (marginHeight + i * charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  2] = (2 * (marginWidth  + j * charWidth  + charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  3] = (2 * (marginHeight + i * charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  4] = (2 * (marginWidth  + j * charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  5] = (2 * (marginHeight + i * charHeight + charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  6] = (2 * (marginWidth  + j * charWidth  + charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  7] = (2 * (marginHeight + i * charHeight + charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    // UVs of each character quad - one for each vert
                    allUVs[STRIDE(8) + 0] = 0;
                    allUVs[STRIDE(8) + 1] = 0;
                    
                    allUVs[STRIDE(8) + 2] = 0;
                    allUVs[STRIDE(8) + 3] = 0;
                    
                    allUVs[STRIDE(8) + 4] = 0;
                    allUVs[STRIDE(8) + 5] = 0;
                    
                    allUVs[STRIDE(8) + 6] = 0;
                    allUVs[STRIDE(8) + 7] = 0;
                    
                    // Two triangles drawn per character quad
                    allIndices[STRIDE(6) + 0] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 1] = STRIDE(4) +  3;
                    allIndices[STRIDE(6) + 2] = STRIDE(4) +  2;
                    
                    allIndices[STRIDE(6) + 3] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 4] = STRIDE(4) +  1;
                    allIndices[STRIDE(6) + 5] = STRIDE(4) +  3;
                }
            }
            
            for (int i = bodyHeightInChars; i < bodyHeightInChars + commandHeightInChars; i++){
                for (int j = 0; j < widthInChars; j++){
                    allVerts[STRIDE(8) +  0] = (2 * (    marginWidth  + j * charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  1] = (2 * (2 * marginHeight + i * charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  2] = (2 * (    marginWidth  + j * charWidth  + charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  3] = (2 * (2 * marginHeight + i * charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  4] = (2 * (    marginWidth  + j * charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  5] = (2 * (2 * marginHeight + i * charHeight + charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  6] = (2 * (    marginWidth  + j * charWidth  + charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  7] = (2 * (2 * marginHeight + i * charHeight + charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    
                    // UVs of each character quad - one for each vert
                    allUVs[STRIDE(8) + 0] = 0;
                    allUVs[STRIDE(8) + 1] = 0;
                    
                    allUVs[STRIDE(8) + 2] = 0;
                    allUVs[STRIDE(8) + 3] = 0;
                    
                    allUVs[STRIDE(8) + 4] = 0;
                    allUVs[STRIDE(8) + 5] = 0;
                    
                    allUVs[STRIDE(8) + 6] = 0;
                    allUVs[STRIDE(8) + 7] = 0;
                    
                    
                    allIndices[STRIDE(6) + 0] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 1] = STRIDE(4) +  3;
                    allIndices[STRIDE(6) + 2] = STRIDE(4) +  2;
                    
                    allIndices[STRIDE(6) + 3] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 4] = STRIDE(4) +  1;
                    allIndices[STRIDE(6) + 5] = STRIDE(4) +  3;
                }
            }
        }
        
        void loadFontToBitmap(){
            
        }
        
    public:
        GUI_Console() {}
        ~GUI_Console() {
            glDeleteBuffers (2, buffers);
            glDeleteProgram (shdrLoc);
            //glDeleteTextures (1, &texture);
            glDeleteVertexArrays (1, &VAOloc);
            delete[] allVerts;
            delete[] allUVs;
            delete[] allIndices;
        }
        
        bool Init(std::stringstream& output, const char* fontFileName, int locX, int locY, int width, int height, int charW, int charH, int spacingX, int spacingY, int marginX, int marginY){
            
            screenOffsetX =  locX;
            screenOffsetY = -locY; // openGL flips y+ is up
            
            charWidth =         charW;
            charHeight = (-1) * charH;   // openGL flips y+ is up
            
            widthInChars = width / charW;
            width = widthInChars * charW;
            bodyHeightInChars = height / charH - commandHeightInChars;
            bodyHeight = bodyHeightInChars * charH;
            commandHeight = commandHeightInChars * charH;
            
            marginWidth =         (width % charW) / 2;
            marginHeight = (-1) * (height % charH) / 3;    // openGL flips y+ is up
            
            shdrLoc = loadShaders("Assets/Shaders/GUI_Console.vert", "Assets/Shaders/GUI_Console.frag", output);

            attrLoc_verts   = glGetAttribLocation(shdrLoc, "Vertex");
            //attrLoc_uvCoo   = glGetAttribLocation(shdrLoc, "UV");
            //txtrLoc         = glGetUniformLocation(shdrLoc, "font_texture");
            
            determineBufferData();
            loadFontToBitmap();
            
            
//            output << "\n" << sizeIndexArray << "\n\n";
//            for (int i = 0; i < bodyHeightInChars + commandHeightInChars; i++){
//                for (int j = 0; j < widthInChars; j++){
//                    output << "[" << allVerts[i * widthInChars * 8 + j * 8 + 0] << ", "
//                                  << allVerts[i * widthInChars * 8 + j * 8 + 1] << "] ["
//                                  << allVerts[i * widthInChars * 8 + j * 8 + 2] << ", "
//                                  << allVerts[i * widthInChars * 8 + j * 8 + 3] << "] ["
//                                  << allVerts[i * widthInChars * 8 + j * 8 + 4] << ", "
//                                  << allVerts[i * widthInChars * 8 + j * 8 + 5] << "] ["
//                                  << allVerts[i * widthInChars * 8 + j * 8 + 6] << ", "
//                                  << allVerts[i * widthInChars * 8 + j * 8 + 7] << "]\n";                    
//                }
//            }
//            output << "\n\n";
//            for (int i = 0; i < bodyHeightInChars + commandHeightInChars; i++){
//                for (int j = 0; j < widthInChars; j++){
//                    output << "[" << allIndices[i * widthInChars * 6 + j * 6 + 0] << ", "
//                                  << allIndices[i * widthInChars * 6 + j * 6 + 1] << ", "
//                                  << allIndices[i * widthInChars * 6 + j * 6 + 2] << "] ["                            
//                                  << allIndices[i * widthInChars * 6 + j * 6 + 3] << ", "
//                                  << allIndices[i * widthInChars * 6 + j * 6 + 4] << ", "
//                                  << allIndices[i * widthInChars * 6 + j * 6 + 5] << "]\n";                    
//                }
//            }
            
            
            // buffer part    
            glGenVertexArrays(1, &VAOloc);
            glBindVertexArray(VAOloc);

            glGenBuffers(2, buffers);

            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(DUA_FLOAT), allVerts, GL_STATIC_DRAW);
            glEnableVertexAttribArray(attrLoc_verts);
            glVertexAttribPointer(attrLoc_verts, 2, GL_FLOAT, GL_FALSE, 0, 0);

//            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
//            glBufferData(GL_ARRAY_BUFFER, sizeof allUVs, allUVs, GL_DYNAMIC_DRAW);
//            glEnableVertexAttribArray(attrLoc_uvCoo);
//            glVertexAttribPointer(attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0); 

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexArray * sizeof(DUA_UINT16), allIndices, GL_STATIC_DRAW);
            
            return true;
        }
        
        void render(){
            
            glUseProgram (shdrLoc);
            
            glBindVertexArray (VAOloc);           
            
            glDrawElements (GL_LINE_LOOP, sizeIndexArray, GL_UNSIGNED_SHORT, 0);
        }
    };
}

#endif	/* GUI_CONSOLE_H */

