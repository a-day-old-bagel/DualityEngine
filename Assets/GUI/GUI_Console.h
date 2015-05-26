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
        
        //glm::mat4 projection = glm::mat4(1.0f);
        
        int charWidth;
        int charHeight;
        int charStepX;
        int charStepY;
        
        int totalWidth;
        int totalHeight;
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
        
//        template<typename type>
//        type findNextPowOfTwo (const type inputNum){
//            type powOfTwo = 2;
//            while ((powOfTwo <<= 1) < inputNum);
//            return powOfTwo;
//        }
        
        bool generateAndBufferGeometry (std::stringstream& output){
            int numBodyVerts = numCharsX * numCharsY_body * 4;
            int numCommVerts = numCharsX * numCharsY_comm * 4;
            int numBodyTris  = numCharsX * numCharsY_body * 2;
            int numCommTris  = numCharsX * numCharsY_comm * 2;
            
            sizeVertArray = (numBodyVerts + numCommVerts + 4) * 2;// +4 and +2 are for background quad.    
            sizeIndexArray = (numBodyTris + numCommTris + 2) * 3;
            
            /*DEBUG*/output << "verts: " << sizeVertArray << " indices: " << sizeIndexArray << "\n";
            
            DUA_float allVerts[sizeVertArray];
            DUA_float allUVs[sizeVertArray];
            DUA_uint16 allIndices[sizeIndexArray];
            
            output <<                screenOffsetX  / (float)DUA_SCREENRES_X - 1 << "\n";
            output <<                screenOffsetY  / (float)DUA_SCREENRES_Y + 1 << "\n";
            
            output << (totalWidth  + screenOffsetX) / (float)DUA_SCREENRES_X - 1 << "\n";
            output <<                screenOffsetY  / (float)DUA_SCREENRES_Y + 1 << "\n";
            
            output <<                screenOffsetX  / (float)DUA_SCREENRES_X - 1 << "\n";
            output << (totalHeight + screenOffsetY) / (float)DUA_SCREENRES_Y + 1 << "\n";
            
            output << (totalWidth  + screenOffsetX) / (float)DUA_SCREENRES_X - 1 << "\n";
            output << (totalHeight + screenOffsetY) / (float)DUA_SCREENRES_Y + 1 << "\n" << "\n";
            
            allVerts[sizeVertArray - 8] =                screenOffsetX  / ((float)DUA_SCREENRES_X * 0.5f) - 1;
            allVerts[sizeVertArray - 7] =                screenOffsetY  / ((float)DUA_SCREENRES_Y * 0.5f) + 1;
            
            allVerts[sizeVertArray - 6] = (totalWidth  + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5f) - 1;
            allVerts[sizeVertArray - 5] =                screenOffsetY  / ((float)DUA_SCREENRES_Y * 0.5f) + 1;
            
            allVerts[sizeVertArray - 4] =                screenOffsetX  / ((float)DUA_SCREENRES_X * 0.5f) - 1;
            allVerts[sizeVertArray - 3] = (totalHeight + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5f) + 1;
            
            allVerts[sizeVertArray - 2] = (totalWidth  + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5f) - 1;
            allVerts[sizeVertArray - 1] = (totalHeight + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5f) + 1;
            
            allUVs[sizeVertArray - 8] = 0.0;//0.0 / (endOfChars - beginningOfChars + 2);
            allUVs[sizeVertArray - 7] = 0.0;//0.0;            
            allUVs[sizeVertArray - 6] = 0.01;//1.0 / (endOfChars - beginningOfChars + 2);
            allUVs[sizeVertArray - 5] = 0.0;            
            allUVs[sizeVertArray - 4] = 0.0;//0.0 / (endOfChars - beginningOfChars + 2);
            allUVs[sizeVertArray - 3] = 1.0;//1.0;            
            allUVs[sizeVertArray - 2] = 0.01;//1.0 / (endOfChars - beginningOfChars + 2);
            allUVs[sizeVertArray - 1] = 1.06;//1.0;
            
            allIndices[sizeIndexArray - 6] = sizeVertArray / 2 - 4;
            allIndices[sizeIndexArray - 5] = sizeVertArray / 2 - 2;
            allIndices[sizeIndexArray - 4] = sizeVertArray / 2 - 1;
            
            allIndices[sizeIndexArray - 3] = sizeVertArray / 2 - 4;
            allIndices[sizeIndexArray - 2] = sizeVertArray / 2 - 1;
            allIndices[sizeIndexArray - 1] = sizeVertArray / 2 - 3;
            
            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                output << "glError creating background quad: " << gluErrorString(glErr) << std::endl;
            }
            
            
            //memset(allUVs, 0, sizeVertArray);
            
            /*DEBUG*/const int debugStartChar = 17;
            /*DEBUG*/const float adv = 1.0 / (endOfChars - beginningOfChars + 2);
            /*DEBUG*/const float startX = adv * debugStartChar;
            /*DEBUG*/const float endX = startX + adv;
            
            // generate vertex and index data for all the character quads, first for the characters in the body text
            #define STRIDE(x) (i * numCharsX * x) + (j * x)// + x // +x is for background quad.
            for (int i = 0; i < numCharsY_body; i++){
                for (int j = 0; j < numCharsX; j++){
                    output << STRIDE(8) << " to " << STRIDE(8) + 7 << " and " << STRIDE(6) << " to " << STRIDE(6) + 5 << "\n";
                    // Vertices of each character quad - four per quad
                    allVerts[STRIDE(8) +  0] = ((marginWidth  + j * charStepX) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    allVerts[STRIDE(8) +  1] = ((marginHeight + i * charStepY) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    allVerts[STRIDE(8) +  2] = ((marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    allVerts[STRIDE(8) +  3] = ((marginHeight + i * charStepY) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    allVerts[STRIDE(8) +  4] = ((marginWidth  + j * charStepX) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    allVerts[STRIDE(8) +  5] = ((marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    allVerts[STRIDE(8) +  6] = ((marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    allVerts[STRIDE(8) +  7] = ((marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    
                    /*DEBUG*/allUVs[STRIDE(8) +  0] = (startX + (i * numCharsX + j) * adv);
                    /*DEBUG*/allUVs[STRIDE(8) +  1] = 0.0;

                    /*DEBUG*/allUVs[STRIDE(8) +  2] = (endX + (i * numCharsX + j) * adv);
                    /*DEBUG*/allUVs[STRIDE(8) +  3] = 0.0;

                    /*DEBUG*/allUVs[STRIDE(8) +  4] = (startX + (i * numCharsX + j) * adv);
                    /*DEBUG*/allUVs[STRIDE(8) +  5] = 1.0;

                    /*DEBUG*/allUVs[STRIDE(8) +  6] = (endX + (i * numCharsX + j) * adv);
                    /*DEBUG*/allUVs[STRIDE(8) +  7] = 1.0;
                    
                   
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
                    output << STRIDE(8) << " to " << STRIDE(8) + 7 << " and " << STRIDE(6) << " to " << STRIDE(6) + 5 << "\n";
                    // Vertices of each character quad - four per quad
                    allVerts[STRIDE(8) +  0] = ((    marginWidth  + j * charStepX) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    allVerts[STRIDE(8) +  1] = ((2 * marginHeight + i * charStepY) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    allVerts[STRIDE(8) +  2] = ((    marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    allVerts[STRIDE(8) +  3] = ((2 * marginHeight + i * charStepY) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    allVerts[STRIDE(8) +  4] = ((    marginWidth  + j * charStepX) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    allVerts[STRIDE(8) +  5] = ((2 * marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    allVerts[STRIDE(8) +  6] = ((    marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    allVerts[STRIDE(8) +  7] = ((2 * marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    
                    /*DEBUG*/allUVs[STRIDE(8) +  0] = (startX + (i * numCharsX + j) * adv);
                    /*DEBUG*/allUVs[STRIDE(8) +  1] = 0.0;

                    /*DEBUG*/allUVs[STRIDE(8) +  2] = (endX + (i * numCharsX + j) * adv);
                    /*DEBUG*/allUVs[STRIDE(8) +  3] = 0.0;

                    /*DEBUG*/allUVs[STRIDE(8) +  4] = (startX + (i * numCharsX + j) * adv);
                    /*DEBUG*/allUVs[STRIDE(8) +  5] = 1.0;

                    /*DEBUG*/allUVs[STRIDE(8) +  6] = (endX + (i * numCharsX + j) * adv);
                    /*DEBUG*/allUVs[STRIDE(8) +  7] = 1.0;
                    
                    
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
            glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(DUA_float), allVerts, GL_STATIC_DRAW);
            glEnableVertexAttribArray(attrLoc_verts);
            glVertexAttribPointer(attrLoc_verts, 2, GL_FLOAT, GL_FALSE, 0, 0);
            // buffer the empty UV data to GPU
            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(DUA_float), allUVs, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(attrLoc_uvCoo);
            glVertexAttribPointer(attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0);
            // buffer the triangle index data to GPU
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexArray * sizeof(DUA_uint16), allIndices, GL_STATIC_DRAW);
            
            return true;
        }
        
        bool generateAndBufferFontAtlas(std::stringstream& output, const char* fontFile){            
            
            FT_Library library;
            FT_Face     face;
            
            const int numCharTexPanels = endOfChars - beginningOfChars + 2;
            int startChar = beginningOfChars - 1;
            int endChar = endOfChars;
            int texWidth = charWidth;//findNextPowOfTwo(charWidth);
            int texHeight = -charHeight;//findNextPowOfTwo(-charHeight);  // REMEMBER stupid openGL y flipping
            /*DEBUG*/int texBytes = numCharTexPanels * texWidth * texHeight;
            

            FT_Error error = FT_Init_FreeType (&library);
            if (error){
                output << "\nERROR initializing freeType library.\n";
                return false;
            }            
            error = FT_New_Face (library, fontFile, 0, &face);
            if (error == FT_Err_Unknown_File_Format){
                output << "\nERROR loading font: unknown file format.\n";
                return false;
            } else if (error){
                output << "\nERROR loading font.\n";
                return false;
            }            
            error = FT_Set_Pixel_Sizes (face, texWidth * 2, texHeight); // extra factor of 2 is just 'cause fonts were looking "skinny."
            if (error){
                output << "\nERROR setting font size.\n";
                return false;
            }            
            
            /*DEBUG*/DUA_uint8 texData[texBytes];
            /*DEBUG*/memset(texData, 0x40, texBytes);            
            
            glGenTextures (1, &texture);
            glBindTexture (GL_TEXTURE_2D, texture);
            glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexImage2D (GL_TEXTURE_2D, 0, GL_RED, texWidth * numCharTexPanels, texHeight, 0, GL_RED, GL_UNSIGNED_BYTE, &texData[0]);/*DEBUG*/ //should be 0 on end
            
            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                output << "glError after creating empty texture atlas.\n" << gluErrorString(glErr) << std::endl;
            }
            
            const int baseline = texHeight * 0.8; // 4/5th of panel height FROM THE TOP (stupid openGL)
            int yOffset;    // this is how far each char will be drawn from the top side of its quad (based on offset from font 'baseline')
            int xOffset;    // this is how far each char will be drawn from the left side of its quad. 
            
            for (char i = startChar; i <= endChar; i++){
                FT_UInt glyph_index = FT_Get_Char_Index(face, (int)i);
               
                error = FT_Load_Glyph (face, glyph_index /*i*/, FT_LOAD_RENDER);
                if (error){
                    output << "\nERROR loading font character " << i << std::endl;
                }
                
                FT_Bitmap bmp = face->glyph->bitmap;
                int bmpW = bmp.width;
                int bmpH = bmp.rows;
                
                if (bmp.buffer == NULL){
                    if (i != ' '){  // because why would there be a bitmap for 'space'?
                        output << "No bitmap loaded for character '" << i << "'.\n";
                    }
                } else {
                    yOffset = baseline - face->glyph->metrics.horiBearingY / 64; // units in 1/64 pixel (stupid freetype)
                    xOffset = face->glyph->metrics.horiBearingX / 64;
                    glTexSubImage2D(GL_TEXTURE_2D, 0, (i - startChar) * texWidth + xOffset, yOffset, bmpW, bmpH, GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);
                }
                
                glErr = glGetError ();
                if (glErr != GL_NO_ERROR){
                    output << "glError while subbing texture for '" << i << "': " << gluErrorString (glErr) << std::endl;
                } else {
                    
                }
            }            
            
            FT_Done_Face (face);
            FT_Done_FreeType (library);
            
            return true;
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
        
        bool Init(std::stringstream& output, const char* fontFile, int locX, int locY, int width, int height, int charW, int charH, int spacingX, int spacingY, int marginX, int marginY){
            
            screenOffsetX =  locX;
            screenOffsetY = -locY; // openGL flips y+ is up
            
            charWidth =         charW;
            charHeight = (-1) * charH;   // openGL flips y+ is up
            
            charStepX = charWidth  + spacingX;
            charStepY = charHeight - spacingY;
            
            totalWidth = width;
            totalHeight = -height;
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
            
            // buffer part    
            glGenVertexArrays(1, &VAOloc);
            glBindVertexArray(VAOloc);
            glGenBuffers(3, buffers);            
            
            if (!generateAndBufferGeometry(output)) return false;
            if (!generateAndBufferFontAtlas(output, fontFile)) return false;
            
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

