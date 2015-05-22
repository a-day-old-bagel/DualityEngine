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
        
        const char beginningOfChars = ' ';
        const char endOfChars = '~';
        
        GLuint buffers[3];
        GLuint texture;
        GLuint VAOloc;
        GLuint shdrLoc;
        GLuint txtrLoc;
        GLuint attrLoc_verts;
        GLuint attrLoc_uvCoo;
        //GLuint unifLoc_txDim;
        
        glm::mat4 projection = glm::mat4(1.0f);
        
        int charWidth;
        int charHeight;
        int charStepX;
        int charStepY;
        
        int innerWidth;
        int innerHeight;
        
        int numCharsX; 
        int numCharsY_body;
        const int numCharsY_comm = 1;
        
        int screenOffsetX;
        int screenOffsetY;
        
        int sizeVertArray;
        int sizeIndexArray;
        
        int marginWidth;
        int marginHeight;
        
        template<typename type>
        type findNextPowOfTwo (const type inputNum){
            type powOfTwo = 2;
            while ((powOfTwo <<= 1) < inputNum);
            return powOfTwo;
        }
        
        bool generateAndBufferGeometry (std::stringstream& output){
            int numBodyVerts = numCharsX * numCharsY_body * 4;
            int numCommVerts = numCharsX * numCharsY_comm * 4;
            int numBodyTris  = numCharsX * numCharsY_body * 2;
            int numCommTris  = numCharsX * numCharsY_comm * 2;
            
            sizeVertArray = (numBodyVerts + numCommVerts) * 2;            
            sizeIndexArray = (numBodyTris + numCommTris) * 3;
            
            DUA_FLOAT allVerts[sizeVertArray];
            DUA_FLOAT allUVs[sizeVertArray];
            DUA_UINT16 allIndices[sizeIndexArray];
            
            //memset(allUVs, 0, sizeVertArray);
            for (int i = 0; i < sizeVertArray; i += 8){
                allUVs[i+0] = 0.0;
                allUVs[i+1] = 1.0;
                
                allUVs[i+2] = 1.0;
                allUVs[i+3] = 1.0;
                
                allUVs[i+4] = 0.0;
                allUVs[i+5] = 0.0;
                
                allUVs[i+6] = 1.0;
                allUVs[i+7] = 0.0;
            }
            
            // generate vertex and index data for all the character quads, first for the characters in the body text
            #define STRIDE(x) i * numCharsX * x + j * x
            for (int i = 0; i < numCharsY_body; i++){
                for (int j = 0; j < numCharsX; j++){                    
                    // Vertices of each character quad - four per quad
                    allVerts[STRIDE(8) +  0] = (2 * (marginWidth  + j * charStepX) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  1] = (2 * (marginHeight + i * charStepY) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  2] = (2 * (marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  3] = (2 * (marginHeight + i * charStepY) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  4] = (2 * (marginWidth  + j * charStepX) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  5] = (2 * (marginHeight + i * charStepY + charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  6] = (2 * (marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  7] = (2 * (marginHeight + i * charStepY + charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                   
                    // Two triangles drawn per character quad
                    allIndices[STRIDE(6) + 0] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 1] = STRIDE(4) +  2;
                    allIndices[STRIDE(6) + 2] = STRIDE(4) +  3;
                    
                    allIndices[STRIDE(6) + 3] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 4] = STRIDE(4) +  3;
                    allIndices[STRIDE(6) + 5] = STRIDE(4) +  1;
                }
            }
            // then do the same thing for the quads belonging to the command input line text
            for (int i = numCharsY_body; i < numCharsY_body + numCharsY_comm; i++){
                for (int j = 0; j < numCharsX; j++){
                    // Vertices of each character quad - four per quad
                    allVerts[STRIDE(8) +  0] = (2 * (    marginWidth  + j * charStepX) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  1] = (2 * (2 * marginHeight + i * charStepY) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  2] = (2 * (    marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  3] = (2 * (2 * marginHeight + i * charStepY) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  4] = (2 * (    marginWidth  + j * charStepX) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  5] = (2 * (2 * marginHeight + i * charStepY + charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    allVerts[STRIDE(8) +  6] = (2 * (    marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / (float)DUA_SCREENRES_X - 1;
                    allVerts[STRIDE(8) +  7] = (2 * (2 * marginHeight + i * charStepY + charHeight) + screenOffsetY) / (float)DUA_SCREENRES_Y + 1;
                    
                    // Two triangles drawn per character quad
                    allIndices[STRIDE(6) + 0] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 1] = STRIDE(4) +  2;
                    allIndices[STRIDE(6) + 2] = STRIDE(4) +  3;
                    
                    allIndices[STRIDE(6) + 3] = STRIDE(4) +  0;
                    allIndices[STRIDE(6) + 4] = STRIDE(4) +  3;
                    allIndices[STRIDE(6) + 5] = STRIDE(4) +  1;
                }
            }
            
            // buffer the vertex data to GPU
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(DUA_FLOAT), allVerts, GL_STATIC_DRAW);
            glEnableVertexAttribArray(attrLoc_verts);
            glVertexAttribPointer(attrLoc_verts, 2, GL_FLOAT, GL_FALSE, 0, 0);
            // buffer the empty UV data to GPU
            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(DUA_FLOAT), allUVs, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(attrLoc_uvCoo);
            glVertexAttribPointer(attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0);
            // buffer the triangle index data to GPU
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexArray * sizeof(DUA_UINT16), allIndices, GL_STATIC_DRAW);
            
            return true;
        }
        
        bool generateAndBufferFontSpriteSheetTexture(std::stringstream& output){
            
            FT_Library library;
            FT_Face     face;
            
            const int numCharTexPanels = endOfChars - beginningOfChars + 2;
            int startChar = beginningOfChars + 1;
            int endChar = endOfChars;
            int texWidth = findNextPowOfTwo(charWidth);
            int texHeight = findNextPowOfTwo(-charHeight)/ 2;  // REMEMBER stupid openGL y flipping
            int texBytes = numCharTexPanels * texWidth * texHeight;
            output << texWidth << " " << texHeight << "\n";

            FT_Error error = FT_Init_FreeType (&library);
            if (error){
                output << "\nERROR initializing freeType library.\n";
                return false;
            }
            
            error = FT_New_Face (library, "Assets/Fonts/FantasqueSansMono-Regular.ttf", 0, &face);
            if (error == FT_Err_Unknown_File_Format){
                output << "\nERROR loading font: unknown file format.\n";
                return false;
            }
            else if (error){
                output << "\nERROR loading font.\n";
                return false;
            }
            
            error = FT_Set_Pixel_Sizes (face, texWidth, texHeight);
            if (error){
                output << "\nERROR setting font size.\n";
                return false;
            }
            
            DUA_UINT8 texData[texBytes];
            memset(texData, 0x0F, texBytes);
            
//            for (int i = 0; i < texHeight; i += 2){
//                for (int j = 0; j < texWidth; j++){
//                    texData[i * texWidth + j + 0] = 0xFF;
//                    texData[i * texWidth + j + 1] = 0x00;
//                }
//            }
//            
//            for (char i = startChar; i <= endChar; i++){
//                FT_UInt glyph_index = FT_Get_Char_Index(face, (int)i);
//               
//                error = FT_Load_Glyph (face, glyph_index /*i*/, FT_LOAD_RENDER);
//                if (error){
//                    output << "\nERROR loading font character " << i << std::endl;
//                    return false;
//                }
//                
//                FT_Bitmap bmp = face->glyph->bitmap;
//                int bmpW = bmp.width;
//                int bmpH = bmp.rows;
//                
//                if (bmp.buffer == NULL){
//                    output << "NULL\n";
//                } else {                    
//                    for (int j = 0; j < bmpH; j++){
//                        for (int k = 0; k < bmpW; k++){
//                            texData[(i - startChar) * texWidth * texHeight + j * bmpW + k] = bmp.buffer[j * bmpW + k];
//                        }
//                    }
//                }                
//            }
//            
//            GLenum glErr = glGetError();
//            if (glErr != GL_NO_ERROR) {
//                output << "\nglError 0: " << gluErrorString(glErr) << std::endl;
//            }
            
            glGenTextures (1, &texture);
            glBindTexture (GL_TEXTURE_2D, texture);
            glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexImage2D (GL_TEXTURE_2D, 0, GL_RED, texWidth, texHeight * numCharTexPanels, 0, GL_RED, GL_UNSIGNED_BYTE, &texData[0]);
            
            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                output << "\nglError 1: " << gluErrorString(glErr) << std::endl;
            }
            
            FT_Done_Face (face);
            FT_Done_FreeType (library);
            
            return true;
            
            
            
            
            
            
//            
//            FT_Library library;
//            FT_Face     face;
//            
//            const int numCharTexPanels = endOfChars - beginningOfChars + 2;
//            int startChar = beginningOfChars + 1;
//            int endChar = endOfChars;
//            int texWidth = findNextPowOfTwo(charWidth);
//            int texHeight = findNextPowOfTwo(-charHeight);  // REMEMBER stupid openGL y flipping
//            int texBytes = numCharTexPanels * texWidth * texHeight;
//            output << texWidth << " " << texHeight << "\n";
//
//            FT_Error error = FT_Init_FreeType (&library);
//            if (error){
//                output << "\nERROR initializing freeType library.\n";
//                return false;
//            }
//            
//            error = FT_New_Face (library, "Assets/Fonts/FantasqueSansMono-Regular.ttf", 0, &face);
//            if (error == FT_Err_Unknown_File_Format){
//                output << "\nERROR loading font: unknown file format.\n";
//                return false;
//            }
//            else if (error){
//                output << "\nERROR loading font.\n";
//                return false;
//            }
//            
//            error = FT_Set_Pixel_Sizes (face, texWidth, texHeight);
//            if (error){
//                output << "\nERROR setting font size.\n";
//                return false;
//            }
//            
//            
//            GLenum glErr = glGetError();
//            if (glErr != GL_NO_ERROR) {
//                output << "\nglError 0: " << gluErrorString(glErr) << std::endl;
//            }
//            
//            
//            DUA_UINT8 texData[texBytes];
//            memset(texData, 0, texBytes);
//            
////            for (int i = 0; i < texBytes; i++){
////                texData[i] = 0x0F;
////            }
//            
//            glGenTextures (1, &texture);
//            glBindTexture (GL_TEXTURE_2D, texture);
//            glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
//            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//            glTexImage2D (GL_TEXTURE_2D, 0, GL_RED, texWidth, texHeight * numCharTexPanels, 0, GL_RED, GL_UNSIGNED_BYTE, &texData[0]);
//            
//            glErr = glGetError();
//            if (glErr != GL_NO_ERROR) {
//                output << "\nglError 1: " << gluErrorString(glErr) << std::endl;
//            }
//            
//            //glTexImage2D (GL_TEXTURE_2D, 0, GL_RED, texWidth * numCharTexPanels, texHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);//&texData[0]);
//            
//            glErr = glGetError();
//            if (glErr != GL_NO_ERROR) {
//                output << "\nglError 2: " << gluErrorString(glErr) << std::endl;
//            }
//            
//            DUA_COLORBYTE unkwnChar[texWidth * texHeight];
//            for (int i = 0; i < texHeight; i += 2){
//                for (int j = 0; j < texWidth; j++){
//                    unkwnChar[i * texWidth + j + 0] = 0xFF;
//                    unkwnChar[i * texWidth + j + 1] = 0x00;
//                }
//            }
//            
//            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texWidth, texHeight, GL_RED, GL_UNSIGNED_BYTE, unkwnChar);
//            
//            glErr = glGetError();
//            if (glErr != GL_NO_ERROR) {
//                output << "\nglError 3: " << gluErrorString(glErr) << std::endl;
//            }
//            
//            for (char i = startChar; i <= endChar; i++){
//                FT_UInt glyph_index = FT_Get_Char_Index(face, (int)i);
//               
//                error = FT_Load_Glyph (face, glyph_index /*i*/, FT_LOAD_RENDER);
//                if (error){
//                    output << "\nERROR loading font character " << i << std::endl;
//                    return false;
//                }
//                
//                FT_Bitmap bmp = face->glyph->bitmap;
//                int bmpW = bmp.width;
//                int bmpH = bmp.rows;
//                
//                if (bmp.buffer == NULL){
//                    output << "NULL\n";
//                } else {                    
//                    glTexSubImage2D(GL_TEXTURE_2D, 0, (i - startChar) * texWidth, 0, bmpW, bmpH, GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);
//                }                
//            }
//            
//            glErr = glGetError();
//            if (glErr != GL_NO_ERROR) {
//                output << "\nglError 4: " << gluErrorString(glErr) << std::endl;
//            }
//            
//            
//            FT_Done_Face (face);
//            FT_Done_FreeType (library);
//            
//            return true;
        }
        
        void updateUVsWithCurrentConsoleText(){
            
        }
        
    public:
        GUI_Console() {}
        ~GUI_Console() {
            glDeleteBuffers (3, buffers);
            glDeleteProgram (shdrLoc);
            glDeleteTextures (1, &texture);
            glDeleteVertexArrays (1, &VAOloc);
        }
        
        bool Init(std::stringstream& output, const char* fontFileName, int locX, int locY, int width, int height, int charW, int charH, int spacingX, int spacingY, int marginX, int marginY){
            
            screenOffsetX =  locX;
            screenOffsetY = -locY; // openGL flips y+ is up
            
            charWidth =         charW;
            charHeight = (-1) * charH;   // openGL flips y+ is up
            
            charStepX = charWidth  + spacingX;
            charStepY = charHeight - spacingY;
            
            innerWidth =         width  - marginX * 2;
            innerHeight = (-1) * height + marginY * 2; // openGL flips y+ is up
            
            numCharsX = innerWidth / charStepX;
            numCharsY_body = innerHeight / charStepY - numCharsY_comm;
            
            marginWidth =         ( innerWidth  %  charStepX) / 2 + marginX;
            marginHeight = (-1) * (-innerHeight % -charStepY) / 3 - marginY;    // openGL flips y+ is up
            
            
            shdrLoc = loadShaders("Assets/Shaders/GUI_Console.vert", "Assets/Shaders/GUI_Console.frag", output);

            attrLoc_verts   = glGetAttribLocation(shdrLoc, "Vertex");
            attrLoc_uvCoo   = glGetAttribLocation(shdrLoc, "UV");
            txtrLoc         = glGetUniformLocation(shdrLoc, "font_texture");
            //unifLoc_txDim   = glGetUniformLocation(shdrLoc, "charTexDimensions");
            
            
            // buffer part    
            glGenVertexArrays(1, &VAOloc);
            glBindVertexArray(VAOloc);
            glGenBuffers(3, buffers);
            
            
            if (!generateAndBufferGeometry(output)) return false;
            if (!generateAndBufferFontSpriteSheetTexture(output)) return false;
            
            
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
            
            return true;
        }
        
        void render(){
            
            glActiveTexture (GL_TEXTURE1);
            glBindTexture (GL_TEXTURE_2D, texture);
            
            glUseProgram (shdrLoc);
            glUniform1i (txtrLoc, 1);
            glBindVertexArray (VAOloc);           
            
            glDrawElements (GL_TRIANGLES, sizeIndexArray, GL_UNSIGNED_SHORT, 0);
        }
    };
}

#endif	/* GUI_CONSOLE_H */

