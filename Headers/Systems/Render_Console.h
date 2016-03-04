/*
 * File:   Render_Console.h
 * Author: adayoldbagel
 *
 * Created on May 19, 2015, 1:47 PM
 *
 * This is a system that, when instantiated and initialized, sets up (generates) all the
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
 *
 *                  It needs more abstraction.  A generic text box or UI should be possible. This is too ad-hoc.
 */

#ifndef RENDERCONSOLE_H
#define	RENDERCONSOLE_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

//<editor-fold defaultstate="collapsed" desc="Includes">

#include "System.h"
#include "Console.h"
#include "Settings.h"
#include "loadShaders.h"
#include <ft2build.h>
#include FT_FREETYPE_H

//</editor-fold>

namespace DualityEngine {

    class System_Render_Console : public System<System_Render_Console>
    {
    private:
        static const glm::vec3 localTextColor;
        static const glm::vec3 localBackColor;
        static const char firstAsciiChar;    // this is the first ascii char representable
        static const char lastAsciiChar;     // and this is the last (look at ascii table)
        static const std::string commPromptNorm; // this appears as the prompt for the console when not in the menu
        static const std::string commPromptMenu; // this appears as the prompt when in the menu
        static const int numTexPanels;
        static const float texPanelAdvance;    // this is how far the GPU texture sampler will have to move to get to the next character in the texture atlas.
        static const int numCharsY_comm;

        bool hasInitialized;
        
        // All of these are openGL-specific fields required to render the console
        GLuint buffers[3];
        GLuint texture;
        GLuint VAOloc_text;
        GLuint shdrLoc;
        GLuint txtrLoc;
        GLuint attrLoc_verts;
        GLuint attrLoc_uvCoo;
        GLuint unifLoc_color;
        
        // This is a pointer to a Console object whose text will be rendered by this system.
        Console* console;
        
        // All of these are re-used each draw as temporary variables to help in the positioning of the char quads
        // and the reading of text from the console object.
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
        
        // These are arrays to hold data that's buffered to the GPU each draw.
        DUA_float* bodySubArray;
        DUA_float* commSubArray;
        DUA_float cursorSubArray[6];
        
        // These are set upon initialization and should be cleaned up some time...
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
        
        // Called once upon initialization, sets vertex positions for the character quads, background quad, and cursor triangle.
        bool generateAndBufferGeometry (std::stringstream& output);
        
        // Called once upon init, creates the texture (image) of the font atlas that is then given to the GPU.
        bool generateAndBufferFontAtlas(std::stringstream& output, const char* fontFile);
        
        // called each draw (as long as text has changed), this calculates the new texture atlas UVs for each char quad.
        void updateBuffersWithCurrentConsoleText();

    public:
        System_Render_Console(Bank * bank, Console* console);
        ~System_Render_Console();
        void tick();
        bool init(std::stringstream& output);
    };

}

#endif


