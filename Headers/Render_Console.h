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
        const glm::vec3 textColor = /*{0.5, 0.8, 1.0};//{1.0, 0.8, 0.0};*/{0.5, 1.0, 0.3};
        const glm::vec3 bkgdColor = /*{0.1, 0.05, 0.0};//{0.0, 0.0, 0.1};*/{0.05, 0.05, 0.05};
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
        
        bool generateAndBufferGeometry (std::stringstream& output);
        bool generateAndBufferFontAtlas(std::stringstream& output, const char* fontFile);
        void updateBuffersWithCurrentConsoleText();

    public:
        System_Render_Console(ComponentBank* bank, Console* console);
        ~System_Render_Console();
        void tick() override;    
        bool init(std::stringstream& output) override;
    };

}

#endif


