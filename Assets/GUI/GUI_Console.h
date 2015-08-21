/* 
 * File:   GUI_Console.h
 * Author: adayoldbagel
 *
 * Created on May 19, 2015, 1:47 PM
 * 
 * This is a class that, when instantiated and initialized, sets up (generates) all the
 * resources needed to display a graphical command line interface (console) to the screen.
 * it can be placed wherever, be of whatever size, of whatever colors, and whatever font.
 * upon instantiation, values are passed for margin sizes (horiz and vert), character spacing
 * (horiz and vert), character sizes in pixels (horiz and vert), position of console quad (x and y),
 * width/height of console quad, and font file to access. Freetype 2 is used to read fonts and
 * convert them to bitmaps.  Monspaced fonts are recommended and some tweaking of this file
 * will probably be required to make any one font look good.
 *      The render() method is called to draw the console each frame.
 *      This class also requires a pointer to a DualityEngine::Console class instance from
 * which it can draw current text, cursor positions, etc.
 * 
 * PROBLEMS TO FIX: right now scrolling (pgUp, pgDown) in the log scrolls by the unwrapped line, 
 *                  meaning that if a single line is displayed across three lines because it was
 *                  too long to fit in one line, pressing pgUp will scroll the whole three lines
 *                  upward instead of the usual one line when that multi-line entry is crossed.
 *                      This is a minor problem and may not need fixing.  So far, all other known
 *                  bugs have been fixed (29 MAY 2015)
 *                      The code is still really messy here though.
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
        
        const char firstAsciiChar = ' ';
        const char lastAsciiChar = '~';
        const std::string commPromptNorm = ">: ";
        const std::string commPromptMenu = "MENU>: ";
        const int numTexPanels = lastAsciiChar - firstAsciiChar + 2; //+2 is for the 'unknown char/background' quad plus the off-by-one.
        const float texPanelAdvance = 1.0 / (float)numTexPanels;    // this is how far the GPU texture sampler will have to move to get to the next character in the texture atlas.
        
        GLuint buffers[3];
        GLuint texture;
        GLuint VAOloc_text;
        GLuint shdrLoc;
        GLuint txtrLoc;
        GLuint attrLoc_verts;
        GLuint attrLoc_uvCoo;
        GLuint unifLoc_color;
        
        Console* console;
        std::string lineReader;
        int lineReaderSize;
        std::string graphicalLine;
        std::string carryOverLine;
        int numLinesCarried = 0;
        int multiLineIterator;
        bool lineHasCarried = false;
        int currentCommLineLength;
        int graphicalCommOffset;
        float cursorZeroPosX;
        float cursorPosY;
        float cursorAdvanceX;
        float cursorHalfWidth;
        float cursorHeight;
        DUA_float* bodySubArray;
        DUA_float* commSubArray;
        DUA_float cursorSubArray[6];
        
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
        const int numCharsY_comm = 1;   // don't change this for now - not completely supported.        
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
            
            sizeVertArray = (numBodyVerts + numCommVerts +4 +3) * 2;// +4 and +2 are for background quad.    
            sizeIndexArray = (numBodyTris + numCommTris +2 +1) * 3;// +3 and +1 are for the cursor triangle.
            
            DUA_float verts[sizeVertArray];
            DUA_float UVs[sizeVertArray];
            DUA_uint16 indices[sizeIndexArray];
            
            // fill the UV buffer with zeros for now.
            memset(UVs, 0, sizeVertArray);
            
            // Generate the backgroud quad
                verts[sizeVertArray - 8] =                screenOffsetX  / ((float)Settings::screenResX * 0.5f) - 1;
                verts[sizeVertArray - 7] =                screenOffsetY  / ((float)Settings::screenResY * 0.5f) + 1;            
                verts[sizeVertArray - 6] = (totalWidth  + screenOffsetX) / ((float)Settings::screenResX * 0.5f) - 1;
                verts[sizeVertArray - 5] =                screenOffsetY  / ((float)Settings::screenResY * 0.5f) + 1;            
                verts[sizeVertArray - 4] =                screenOffsetX  / ((float)Settings::screenResX * 0.5f) - 1;
                verts[sizeVertArray - 3] = (totalHeight + screenOffsetY) / ((float)Settings::screenResY * 0.5f) + 1;            
                verts[sizeVertArray - 2] = (totalWidth  + screenOffsetX) / ((float)Settings::screenResX * 0.5f) - 1;
                verts[sizeVertArray - 1] = (totalHeight + screenOffsetY) / ((float)Settings::screenResY * 0.5f) + 1;

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
                
            // Generate the cursor triangle
                verts[sizeVertArray - 14] =  cursorZeroPosX;
                verts[sizeVertArray - 13] =  cursorPosY;
                verts[sizeVertArray - 12] =  cursorZeroPosX - cursorHalfWidth;
                verts[sizeVertArray - 11] =  cursorPosY - cursorHeight;
                verts[sizeVertArray - 10] =  cursorZeroPosX + cursorHalfWidth;
                verts[sizeVertArray -  9] =  cursorPosY - cursorHeight;
                
                UVs[sizeVertArray - 14] = (1.0 - offSetToCenterX) / numTexPanels;
                UVs[sizeVertArray - 13] =  1.0 - offSetToCenterY;
                UVs[sizeVertArray - 12] =        offSetToCenterX  / numTexPanels;
                UVs[sizeVertArray - 11] =  1.0 - offSetToCenterY;
                UVs[sizeVertArray - 10] = (1.0 - offSetToCenterX) / numTexPanels;
                UVs[sizeVertArray -  9] =        offSetToCenterY;
                
                indices[sizeIndexArray - 9] = sizeVertArray / 2 - 7;
                indices[sizeIndexArray - 8] = sizeVertArray / 2 - 6;
                indices[sizeIndexArray - 7] = sizeVertArray / 2 - 5;
            
            // generate vertex and index data for all the character quads, first for the characters in the body text
            #define STRIDE(x) (i * numCharsX * x) + (j * x)// + x // +x is for background quad.
            for (int i = 0; i < numCharsY_body; i++){
                for (int j = 0; j < numCharsX; j++){
                    // Vertices of each character quad - four per quad
                    verts[STRIDE(8) +  0] = ((marginWidth  + j * charStepX) + screenOffsetX) / ((float)Settings::screenResX * 0.5) - 1;
                    verts[STRIDE(8) +  1] = ((marginHeight + i * charStepY) + screenOffsetY) / ((float)Settings::screenResY * 0.5) + 1;
                    
                    verts[STRIDE(8) +  2] = ((marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)Settings::screenResX * 0.5) - 1;
                    verts[STRIDE(8) +  3] = ((marginHeight + i * charStepY) + screenOffsetY) / ((float)Settings::screenResY * 0.5) + 1;
                    
                    verts[STRIDE(8) +  4] = ((marginWidth  + j * charStepX) + screenOffsetX) / ((float)Settings::screenResX * 0.5) - 1;
                    verts[STRIDE(8) +  5] = ((marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)Settings::screenResY * 0.5) + 1;
                    
                    verts[STRIDE(8) +  6] = ((marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)Settings::screenResX * 0.5) - 1;
                    verts[STRIDE(8) +  7] = ((marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)Settings::screenResY * 0.5) + 1;
                   
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
                    // Vertices of each character quad - four per quad
                    verts[STRIDE(8) +  0] = ((    marginWidth  + j * charStepX) + screenOffsetX) / ((float)Settings::screenResX * 0.5) - 1;
                    verts[STRIDE(8) +  1] = ((2 * marginHeight + i * charStepY) + screenOffsetY) / ((float)Settings::screenResY * 0.5) + 1;
                    
                    verts[STRIDE(8) +  2] = ((    marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)Settings::screenResX * 0.5) - 1;
                    verts[STRIDE(8) +  3] = ((2 * marginHeight + i * charStepY) + screenOffsetY) / ((float)Settings::screenResY * 0.5) + 1;
                    
                    verts[STRIDE(8) +  4] = ((    marginWidth  + j * charStepX) + screenOffsetX) / ((float)Settings::screenResX * 0.5) - 1;
                    verts[STRIDE(8) +  5] = ((2 * marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)Settings::screenResY * 0.5) + 1;
                    
                    verts[STRIDE(8) +  6] = ((    marginWidth  + j * charStepX  + charWidth) + screenOffsetX) / ((float)Settings::screenResX * 0.5) - 1;
                    verts[STRIDE(8) +  7] = ((2 * marginHeight + i * charStepY + charHeight) + screenOffsetY) / ((float)Settings::screenResY * 0.5) + 1;
                    
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
        
        void updateBuffersWithCurrentConsoleText(){
            if (console->bodyHasChangedVisually){
                console->bodyHasChangedVisually = false;
                numLinesCarried = 0;
                lineHasCarried = false;
                graphicalLine.clear();
                for (int i = 0; i < numCharsY_body; i++){
                    if (lineHasCarried){
                        lineHasCarried = false;
                        lineReader = carryOverLine;
                    } else {
                        lineReader = console->getLogLineFromBack(i - numLinesCarried + console->logLineTraverser);
                    }
                    lineReaderSize = lineReader.length();
                    
                    if (lineReaderSize > numCharsX){
                        lineHasCarried = true;
                        numLinesCarried++;
                        for (multiLineIterator = 0; multiLineIterator + numCharsX < lineReaderSize; multiLineIterator += numCharsX);
                        carryOverLine = lineReader.substr(0, multiLineIterator);
                        lineReader = lineReader.substr(multiLineIterator);
                    }
                    
                    lineReaderSize = lineReader.length();
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
                    currentCommLineLength = numCharsX - commPromptMenu.length();
                } else {
                    currentCommLineLength = numCharsX - commPromptNorm.length();
                }
                
                
                
                if (console->cursorPosition > currentCommLineLength - 2){
                    graphicalCommOffset = console->cursorPosition - (currentCommLineLength - 2);
                    lineReader = (console->getPendingCommand()).substr(graphicalCommOffset);
                } else {
                    graphicalCommOffset = 0;
                    lineReader = console->getPendingCommand();
                }
                
                lineReaderSize = lineReader.length();
                
                graphicalLine.replace(0, currentCommLineLength, lineReader);
                if (lineReaderSize < currentCommLineLength) {
                    graphicalLine.replace(lineReaderSize, currentCommLineLength - lineReaderSize, currentCommLineLength - lineReaderSize, ' ');
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
                
                cursorSubArray[0] = cursorZeroPosX + (console->cursorPosition - graphicalCommOffset + (console->menuIsActive ? commPromptMenu.length() : commPromptNorm.size())) * cursorAdvanceX;
                cursorSubArray[2] = cursorSubArray[0] - cursorHalfWidth;
                cursorSubArray[4] = cursorSubArray[0] + cursorHalfWidth;
                
                glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
                glBufferSubData(GL_ARRAY_BUFFER, (sizeVertArray - 14) * sizeof(DUA_float), 6 * sizeof(DUA_float), &cursorSubArray[0]);
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
            
            output << "\nBeginning initialization of console GUI.\n";
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
            
            cursorHalfWidth = (float)charW / (float)Settings::screenResX;
            cursorHeight = (float)charH / (float)Settings::screenResY;
            cursorZeroPosX = (screenOffsetX + marginWidth/* + charStepX * commPromptNorm.size()*/) / ((float)Settings::screenResX * 0.5f) - 1;
            cursorAdvanceX = (charStepX) / ((float)Settings::screenResX * 0.5);
            cursorPosY = (screenOffsetY + totalHeight - marginHeight) / ((float)Settings::screenResY * 0.5f) + 1;
            
            cursorSubArray[1] = cursorPosY;
            cursorSubArray[3] = cursorPosY - cursorHeight;
            cursorSubArray[5] = cursorPosY - cursorHeight;
            
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
            
            output << "Console GUI has initialized.\n\n";
            return true;
        }
        
        void render(){
            
            if (console->consoleIsActive || console->menuIsActive){
                updateBuffersWithCurrentConsoleText();
                
                glActiveTexture (GL_TEXTURE1);
                glBindTexture (GL_TEXTURE_2D, texture);

                glUseProgram (shdrLoc);
                glUniform1i (txtrLoc, 1);
                glBindVertexArray (VAOloc_text);  

                glUniform3fv(unifLoc_color, 1, &Settings::Console::backColor[0]);
                glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, DUA_GL_BUFFER_OFFSET((sizeIndexArray - 6) * sizeof(DUA_uint16)));
                glUniform3fv(unifLoc_color, 1, &Settings::Console::textColor[0]);
                glDrawElements (GL_TRIANGLES, sizeIndexArray - 6, GL_UNSIGNED_SHORT, 0);
            }
        }
    };
}

#endif	/* GUI_CONSOLE_H */

