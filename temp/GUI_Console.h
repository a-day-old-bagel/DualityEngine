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
 */

#ifndef GUI_CONSOLE_H
#define	GUI_CONSOLE_H

#include <sstream>
#include "settings.h"
#include "loaders/loadShaders.h"
#include "Console.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace DualityEngine {
    class GUI_Console {
        
        static const char firstAsciiChar;
        static const char lastAsciiChar;
        static const std::string commPromptNorm;
        static const std::string commPromptMenu;
        static const int numTexPanels;
        static const float texPanelAdvance;
        static const int numCharsY_comm;
        
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
        int numLinesCarried;
        int multiLineIterator;
        bool lineHasCarried;
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
        int screenOffsetX;
        int screenOffsetY;        
        int sizeVertArray;
        int sizeIndexArray;        
        int marginWidth;
        int marginHeight;

        bool generateAndBufferGeometry (std::stringstream& output);
        bool generateAndBufferFontAtlas(std::stringstream& output, const char* fontFile);
        void updateBuffersWithCurrentConsoleText();
        
    public:
        GUI_Console();
        ~GUI_Console();
        bool Init(std::stringstream& output, Console* console, const char* fontFile,
                  int locX, int locY, int width, int height, int charW, int charH,
                  int spacingX, int spacingY, int marginX, int marginY);
        void render();
    };
}

#endif	/* GUI_CONSOLE_H */

