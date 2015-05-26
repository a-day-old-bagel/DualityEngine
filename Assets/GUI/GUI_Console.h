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
#include "../../Headers/Console.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace DualityEngine {
    class GUI_Console {
        
        Console* console;
        const glm::vec3 textColor = {0.5, 1.0, 0.3};
        const glm::vec3 bkgdColor = {0.1, 0.1, 0.1};
        const char firstAsciiChar = ' ';
        const char lastAsciiChar = '~';
        const int numTexPanels = lastAsciiChar - firstAsciiChar + 2;
        const float texPanelAdvance = 1.0 / (float)numTexPanels;
        
        GLuint buffers[3];
        GLuint texture;
        GLuint VAOloc_text;
        GLuint shdrLoc;
        GLuint txtrLoc;
        GLuint attrLoc_verts;
        GLuint attrLoc_uvCoo;
        GLuint unifLoc_color;
        
        std::string lineReader;
        std::string graphicalLine;
        const std::string commPromptNorm = ">: ";
        const std::string commPromptMenu = "MENU>: ";
        int currentPromptLength;
        DUA_float* bodySubArray;
        DUA_float* commSubArray;
        int lineReaderSize;
        
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
        
        bool generateAndBufferGeometry (std::stringstream& output){
            int numBodyVerts = numCharsX * numCharsY_body * 4;
            int numCommVerts = numCharsX * numCharsY_comm * 4;
            int numBodyTris  = numCharsX * numCharsY_body * 2;
            int numCommTris  = numCharsX * numCharsY_comm * 2;
            
            sizeVertArray = (numBodyVerts + numCommVerts + 4) * 2;// +4 and +2 are for background quad.    
            sizeIndexArray = (numBodyTris + numCommTris + 2) * 3;
            
            DUA_float verts[sizeVertArray];
            DUA_float UVs[sizeVertArray];
            DUA_uint16 indices[sizeIndexArray];
            
            // Generate the backgroud quad
                verts[sizeVertArray - 8] =                screenOffsetX  / ((float)DUA_SCREENRES_X * 0.5f) - 1;
                verts[sizeVertArray - 7] =                screenOffsetY  / ((float)DUA_SCREENRES_Y * 0.5f) + 1;            
                verts[sizeVertArray - 6] = (totalWidth  + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5f) - 1;
                verts[sizeVertArray - 5] =                screenOffsetY  / ((float)DUA_SCREENRES_Y * 0.5f) + 1;            
                verts[sizeVertArray - 4] =                screenOffsetX  / ((float)DUA_SCREENRES_X * 0.5f) - 1;
                verts[sizeVertArray - 3] = (totalHeight + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5f) + 1;            
                verts[sizeVertArray - 2] = (totalWidth  + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5f) - 1;
                verts[sizeVertArray - 1] = (totalHeight + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5f) + 1;

                const float offSetToCenterX = ((float)charWidth - 2) / (float)charWidth;
                const float offSetToCenterY = ((float)charHeight + 2) / (float)charHeight;
                UVs[sizeVertArray - 8] = (1.0 - offSetToCenterX) / numTexPanels;
                UVs[sizeVertArray - 7] =  1.0 - offSetToCenterY;            
                UVs[sizeVertArray - 6] =        offSetToCenterX  / numTexPanels;
                UVs[sizeVertArray - 5] =  1.0 - offSetToCenterY;            
                UVs[sizeVertArray - 4] = (1.0 - offSetToCenterX) / numTexPanels;
                UVs[sizeVertArray - 3] =        offSetToCenterY;            
                UVs[sizeVertArray - 2] =        offSetToCenterX  / numTexPanels;
                UVs[sizeVertArray - 1] =        offSetToCenterY;

                indices[sizeIndexArray - 6] = sizeVertArray / 2 - 4;
                indices[sizeIndexArray - 5] = sizeVertArray / 2 - 2;
                indices[sizeIndexArray - 4] = sizeVertArray / 2 - 1;            
                indices[sizeIndexArray - 3] = sizeVertArray / 2 - 4;
                indices[sizeIndexArray - 2] = sizeVertArray / 2 - 1;
                indices[sizeIndexArray - 1] = sizeVertArray / 2 - 3;            
            
            //memset(allUVs, 0, sizeVertArray);
            
            /*DEBUG*/const int debugStartChar = 0;
            /*DEBUG*/const float adv = 1.0 / (lastAsciiChar - firstAsciiChar + 2);
            /*DEBUG*/const float startX = adv * debugStartChar;
            /*DEBUG*/const float endX = startX + adv;
            
            // generate vertex and index data for all the character quads, first for the characters in the body text
            #define STRIDE(x) (i * numCharsX * x) + (j * x)// + x // +x is for background quad.
            for (int i = 0; i < numCharsY_body; i++){
                for (int j = 0; j < numCharsX; j++){
                    //output << STRIDE(8) << " to " << STRIDE(8) + 7 << " and " << STRIDE(6) << " to " << STRIDE(6) + 5 << "\n";
                    // Vertices of each character quad - four per quad
                    verts[STRIDE(8) +  0] = ((marginWidth  + j * charStepX) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    verts[STRIDE(8) +  1] = ((marginHeight + i * charStepY) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    verts[STRIDE(8) +  2] = ((marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    verts[STRIDE(8) +  3] = ((marginHeight + i * charStepY) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    verts[STRIDE(8) +  4] = ((marginWidth  + j * charStepX) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    verts[STRIDE(8) +  5] = ((marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    verts[STRIDE(8) +  6] = ((marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    verts[STRIDE(8) +  7] = ((marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    
                    /*DEBUG*/UVs[STRIDE(8) +  0] = (startX + (i * numCharsX + j) * adv);
                    /*DEBUG*/UVs[STRIDE(8) +  1] = 0.0;

                    /*DEBUG*/UVs[STRIDE(8) +  2] = (endX + (i * numCharsX + j) * adv);
                    /*DEBUG*/UVs[STRIDE(8) +  3] = 0.0;

                    /*DEBUG*/UVs[STRIDE(8) +  4] = (startX + (i * numCharsX + j) * adv);
                    /*DEBUG*/UVs[STRIDE(8) +  5] = 1.0;

                    /*DEBUG*/UVs[STRIDE(8) +  6] = (endX + (i * numCharsX + j) * adv);
                    /*DEBUG*/UVs[STRIDE(8) +  7] = 1.0;
                    
                   
                    // Two triangles drawn per character quad
                    indices[STRIDE(6) + 0] = STRIDE(4) +  0;
                    indices[STRIDE(6) + 1] = STRIDE(4) +  2;
                    indices[STRIDE(6) + 2] = STRIDE(4) +  3;
                    
                    indices[STRIDE(6) + 3] = STRIDE(4) +  0;
                    indices[STRIDE(6) + 4] = STRIDE(4) +  3;
                    indices[STRIDE(6) + 5] = STRIDE(4) +  1;
                }
            }
            
            // then do the same thing for the quads belonging to the command input line text
            for (int i = numCharsY_body; i < numCharsY_body + numCharsY_comm; i++){
                for (int j = 0; j < numCharsX; j++){
                    //output << STRIDE(8) << " to " << STRIDE(8) + 7 << " and " << STRIDE(6) << " to " << STRIDE(6) + 5 << "\n";
                    // Vertices of each character quad - four per quad
                    verts[STRIDE(8) +  0] = ((    marginWidth  + j * charStepX) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    verts[STRIDE(8) +  1] = ((2 * marginHeight + i * charStepY) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    verts[STRIDE(8) +  2] = ((    marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    verts[STRIDE(8) +  3] = ((2 * marginHeight + i * charStepY) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    verts[STRIDE(8) +  4] = ((    marginWidth  + j * charStepX) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    verts[STRIDE(8) +  5] = ((2 * marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    verts[STRIDE(8) +  6] = ((    marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)DUA_SCREENRES_X * 0.5) - 1;
                    verts[STRIDE(8) +  7] = ((2 * marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)DUA_SCREENRES_Y * 0.5) + 1;
                    
                    
                    /*DEBUG*/UVs[STRIDE(8) +  0] = (startX + (i * numCharsX + j) * adv);
                    /*DEBUG*/UVs[STRIDE(8) +  1] = 0.0;

                    /*DEBUG*/UVs[STRIDE(8) +  2] = (endX + (i * numCharsX + j) * adv);
                    /*DEBUG*/UVs[STRIDE(8) +  3] = 0.0;

                    /*DEBUG*/UVs[STRIDE(8) +  4] = (startX + (i * numCharsX + j) * adv);
                    /*DEBUG*/UVs[STRIDE(8) +  5] = 1.0;

                    /*DEBUG*/UVs[STRIDE(8) +  6] = (endX + (i * numCharsX + j) * adv);
                    /*DEBUG*/UVs[STRIDE(8) +  7] = 1.0;
                    
                    
                    // Two triangles drawn per character quad
                    indices[STRIDE(6) + 0] = STRIDE(4) +  0;
                    indices[STRIDE(6) + 1] = STRIDE(4) +  2;
                    indices[STRIDE(6) + 2] = STRIDE(4) +  3;
                    
                    indices[STRIDE(6) + 3] = STRIDE(4) +  0;
                    indices[STRIDE(6) + 4] = STRIDE(4) +  3;
                    indices[STRIDE(6) + 5] = STRIDE(4) +  1;
                }
            }
            
            // buffer the vertex data to GPU
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(DUA_float), verts, GL_STATIC_DRAW);
            glEnableVertexAttribArray(attrLoc_verts);
            glVertexAttribPointer(attrLoc_verts, 2, GL_FLOAT, GL_FALSE, 0, 0);
            // buffer the empty UV data to GPU
            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(DUA_float), UVs, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(attrLoc_uvCoo);
            glVertexAttribPointer(attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0);
            // buffer the triangle index data to GPU
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndexArray * sizeof(DUA_uint16), indices, GL_STATIC_DRAW);
            
            return true;
        }
        
        bool generateAndBufferFontAtlas(std::stringstream& output, const char* fontFile){            
            
            FT_Library library;
            FT_Face     face;
            
            int panelWidth = charWidth;
            int panelHeight = -charHeight;  // REMEMBER stupid openGL y flipping - texHeight is positive, charHeight is negative...
            const int baseline = panelHeight * 0.8; // 4/5th of panel height FROM THE TOP (stupid openGL)
            int yOffset;    // this is how far each char will be drawn from the top side of its quad (based on offset from font 'baseline')
            int xOffset;    // this is how far each char will be drawn from the left side of its quad.
            float xPanelStart;
            int texBytes = numTexPanels * panelWidth * panelHeight;            

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
            error = FT_Set_Pixel_Sizes (face, panelWidth * 1.8, panelHeight); // extra factor in width is just 'cause fonts were looking "skinny." Will need modification to use other fonts.
            if (error){
                output << "\nERROR setting font size.\n";
                return false;
            }            
            
            DUA_uint8 baseTexData[texBytes];
            memset(baseTexData, 0x00, texBytes);            
            
            glGenTextures (1, &texture);
            glBindTexture (GL_TEXTURE_2D, texture);
            glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D (GL_TEXTURE_2D, 0, GL_RED, panelWidth * numTexPanels, panelHeight, 0, GL_RED, GL_UNSIGNED_BYTE, &baseTexData[0]);
            
            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                output << "glError after creating empty texture atlas.\n" << gluErrorString(glErr) << std::endl;
            }
            
            DUA_colorByte firstPanel[panelWidth * panelHeight];
            for (int i = 0; i < panelHeight; i++){
                for (int j = 0; j < panelWidth; j++){
                    if (i == 0 || i == panelHeight - 1 || j == 0 || j == panelWidth - 1){
                        firstPanel[i * panelWidth + j] = 0xFF;
                    } else {
                        firstPanel[i * panelWidth + j] = 0xA0;
                    }
                }
            }
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, panelWidth, panelHeight, GL_RED, GL_UNSIGNED_BYTE, firstPanel);
            
            for (char i = firstAsciiChar; i <= lastAsciiChar; i++){
                FT_UInt glyph_index = FT_Get_Char_Index(face, (int)i);
               
                error = FT_Load_Glyph (face, glyph_index /*i*/, FT_LOAD_RENDER);
                if (error){
                    output << "\nERROR loading font character " << i << std::endl;
                    glTexSubImage2D(GL_TEXTURE_2D, 0, (i - firstAsciiChar + 1) * panelWidth, 0, panelWidth, panelHeight, GL_RED, GL_UNSIGNED_BYTE, firstPanel);
                    continue;
                }
                
                FT_Bitmap bmp = face->glyph->bitmap;
                int bmpW = bmp.width;
                int bmpH = bmp.rows;
       
                if (bmp.buffer == NULL){
                    if (i != ' '){  // because why would there be a bitmap for 'space'?
                        output << "No bitmap loaded for character '" << i << "'.\n";
                        glTexSubImage2D(GL_TEXTURE_2D, 0, (i - firstAsciiChar + 1) * panelWidth, 0, panelWidth, panelHeight, GL_RED, GL_UNSIGNED_BYTE, firstPanel);
                        continue;
                    }
                } else {
                    yOffset = baseline - face->glyph->metrics.horiBearingY / 64; // units in 1/64 pixel (stupid freetype)
                    xOffset = face->glyph->metrics.horiBearingX / 64;
                    xPanelStart = (i - firstAsciiChar + 1) * panelWidth + xOffset;
                    glTexSubImage2D(GL_TEXTURE_2D, 0, xPanelStart, std::max(yOffset, 0),  bmpW, std::min({bmpH, panelHeight - yOffset, panelHeight}), GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);
                }
                
                glErr = glGetError ();
                if (glErr != GL_NO_ERROR){
                    
                    xPanelStart = (i - firstAsciiChar + 1) * panelWidth + (panelWidth - bmpW);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, xPanelStart, std::max(yOffset, 0), bmpW, std::min({bmpH, panelHeight - yOffset, panelHeight}), GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);
                    
                    glErr = glGetError ();
                    if (glErr != GL_NO_ERROR){
                        output << "glError while subbing texture for '" << i << "': " << gluErrorString (glErr) << std::endl;
                        glTexSubImage2D(GL_TEXTURE_2D, 0, (i - firstAsciiChar + 1) * panelWidth, 0, panelWidth, panelHeight, GL_RED, GL_UNSIGNED_BYTE, firstPanel);
                        continue;
                    }
                }
            }            
            
            FT_Done_Face (face);
            FT_Done_FreeType (library);
            
            return true;
        }
        
        void updateUVsWithCurrentConsoleText(){
            if (console->bodyHasChangedVisually){
                console->bodyHasChangedVisually = false;
                graphicalLine.clear();
                for (int i = 0; i < numCharsY_body; i++){
                    lineReader = console->getLogLineFromBack(i);
                    lineReaderSize = lineReader.size();
                    graphicalLine.replace(0, numCharsX, lineReader);
                    if (lineReaderSize < numCharsX){
                        graphicalLine.replace(lineReaderSize, numCharsX - lineReaderSize, numCharsX - lineReaderSize, ' ');
                    }
                    for (int j = 0; j < numCharsX; j++){
                        bodySubArray[(numCharsY_body - i - 1) * numCharsX * 8 + j * 8 + 0] = (graphicalLine[j] - firstAsciiChar + 1) * texPanelAdvance;
                        bodySubArray[(numCharsY_body - i - 1) * numCharsX * 8 + j * 8 + 1] = 0.0;
                        bodySubArray[(numCharsY_body - i - 1) * numCharsX * 8 + j * 8 + 2] = (graphicalLine[j] - firstAsciiChar + 1) * texPanelAdvance + texPanelAdvance;
                        bodySubArray[(numCharsY_body - i - 1) * numCharsX * 8 + j * 8 + 3] = 0.0;
                        bodySubArray[(numCharsY_body - i - 1) * numCharsX * 8 + j * 8 + 4] = (graphicalLine[j] - firstAsciiChar + 1) * texPanelAdvance;
                        bodySubArray[(numCharsY_body - i - 1) * numCharsX * 8 + j * 8 + 5] = 1.0;
                        bodySubArray[(numCharsY_body - i - 1) * numCharsX * 8 + j * 8 + 6] = (graphicalLine[j] - firstAsciiChar + 1) * texPanelAdvance + texPanelAdvance;
                        bodySubArray[(numCharsY_body - i - 1) * numCharsX * 8 + j * 8 + 7] = 1.0;
                    }
                }
                glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, numCharsX * numCharsY_body * 8 * sizeof(DUA_float), &bodySubArray[0]);
            }
            if (console->commHasChangedVisually){           // COMMAND LINE UPDATES NOT READY FOR MULTI-LINE !!!
                console->commHasChangedVisually = false;
                graphicalLine.clear();
                
                if (console->menuIsActive){
                    currentPromptLength = numCharsX - commPromptMenu.size();
                } else {
                    currentPromptLength = numCharsX - commPromptNorm.size();
                }
                
                lineReader = console->getPendingCommand();
                lineReaderSize = lineReader.size();
                graphicalLine.replace(0, currentPromptLength, lineReader);
                if (lineReaderSize < currentPromptLength) {
                    graphicalLine.replace(lineReaderSize, currentPromptLength - lineReaderSize, currentPromptLength - lineReaderSize, ' ');
                }
                graphicalLine.insert(0, console->menuIsActive ? commPromptMenu : commPromptNorm);
                for (int j = 0; j < numCharsX; j++) {
                    commSubArray[j * 8 + 0] = (graphicalLine[j] - firstAsciiChar + 1) * texPanelAdvance;
                    commSubArray[j * 8 + 1] = 0.0;
                    commSubArray[j * 8 + 2] = (graphicalLine[j] - firstAsciiChar + 1) * texPanelAdvance + texPanelAdvance;
                    commSubArray[j * 8 + 3] = 0.0;
                    commSubArray[j * 8 + 4] = (graphicalLine[j] - firstAsciiChar + 1) * texPanelAdvance;
                    commSubArray[j * 8 + 5] = 1.0;
                    commSubArray[j * 8 + 6] = (graphicalLine[j] - firstAsciiChar + 1) * texPanelAdvance + texPanelAdvance;
                    commSubArray[j * 8 + 7] = 1.0;
                }
                
                glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
                glBufferSubData(GL_ARRAY_BUFFER, numCharsX * numCharsY_body * 8 * sizeof(DUA_float), numCharsX * numCharsY_comm * 8 * sizeof(DUA_float), &commSubArray[0]);
            }
            
            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                std::cout << "bad poop error. " << gluErrorString(glErr) << std::endl;
            }
            
        }
        
    public:
        GUI_Console() {}
        ~GUI_Console() {
            glDeleteBuffers (3, buffers);
            glDeleteProgram (shdrLoc);
            glDeleteTextures (1, &texture);
            glDeleteVertexArrays (1, &VAOloc_text);
            delete[] bodySubArray;
            delete[] commSubArray;
        }
        
        bool Init(std::stringstream& output, Console* console, const char* fontFile, int locX, int locY, int width, int height, int charW, int charH, int spacingX, int spacingY, int marginX, int marginY){
            
            this->console = console;
            
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
            
            bodySubArray = new DUA_float[numCharsX * numCharsY_body * 8];
            commSubArray = new DUA_float[numCharsX * numCharsY_comm * 8];
            
            
            shdrLoc = loadShaders("Assets/Shaders/GUI_Console.vert", "Assets/Shaders/GUI_Console.frag", output);

            attrLoc_verts   = glGetAttribLocation(shdrLoc, "Vertex");
            attrLoc_uvCoo   = glGetAttribLocation(shdrLoc, "UV");
            txtrLoc         = glGetUniformLocation(shdrLoc, "font_texture");
            unifLoc_color   = glGetUniformLocation(shdrLoc, "penColor");
            
            if (!generateAndBufferFontAtlas(output, fontFile)) return false;
            
            // buffer part    
            glGenVertexArrays(1, &VAOloc_text);
            glBindVertexArray(VAOloc_text);
            glGenBuffers(3, buffers);            
            
            if (!generateAndBufferGeometry(output)) return false;
            
            return true;
        }
        
        void render(){
            
            if (console->consoleIsActive || console->menuIsActive){
                updateUVsWithCurrentConsoleText();
                
                glActiveTexture (GL_TEXTURE1);
                glBindTexture (GL_TEXTURE_2D, texture);

                glUseProgram (shdrLoc);
                glUniform1i (txtrLoc, 1);
                glBindVertexArray (VAOloc_text);  

                glUniform3fv(unifLoc_color, 1, &bkgdColor[0]);
                glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, BUFFER_OFFSET((sizeIndexArray - 6) * sizeof(DUA_uint16)));
                glUniform3fv(unifLoc_color, 1, &textColor[0]);
                glDrawElements (GL_TRIANGLES, sizeIndexArray - 6, GL_UNSIGNED_SHORT, 0);
            }
        }
    };
}

#endif	/* GUI_CONSOLE_H */

