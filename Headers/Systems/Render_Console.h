/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
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

    class System_Render_Console : public System
    {
    private:
        const glm::vec3 localTextColor = {0.5, 1.0, 0.3};
        const glm::vec3 localBackColor = {0.05, 0.05, 0.05};
        const char firstAsciiChar = ' ';    // this is the first ascii char representable
        const char lastAsciiChar = '~';     // and this is the last (look at ascii table)
        const std::string commPromptNorm = ">: "; // this appears as the prompt for the console when not in the menu
        const std::string commPromptMenu = "MENU>: "; // this appears as the prompt when in the menu
        const int numTexPanels = lastAsciiChar - firstAsciiChar + 2; //+2 is for the 'unknown char/background' quad plus the off-by-one.
        const float texPanelAdvance = 1.f / (float)numTexPanels;    // this is how far the GPU texture sampler will have to move to get to the next character in the texture atlas.
        bool hasInitialized = false;
        
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
        const int numCharsY_comm = 1;   // don't change this for now - multi-line command line not completely supported yet (if ever).        
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
        System_Render_Console(ComponentBank* bank, Console* console);
        ~System_Render_Console();
        void tick() override;    
        bool init(std::stringstream& output) override;
    };

}

#endif


