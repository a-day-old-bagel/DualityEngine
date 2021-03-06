/* 
 * File:   Settings.h
 * Author: adayoldbagel
 *
 * Created on January 29, 2015, 2:18 PM
 * 
 * Contains all the typedefs, defines, enumerators, and extern variables used by Duality
 */

#ifndef DUA_SETTINGS_H
#define	DUA_SETTINGS_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <cstdio>

#ifdef _WIN32
typedef unsigned int uint;
#endif

namespace DualityEngine {
        
    /* SWITCHES AND QUICK SETTINGS  --MAY MODIFY--  ========================= */
    
//    #define DUA_DEBUG_CONSOLE_TO_COUT // all console output will be duplicated and sent to stdout
    
//    #define DUA_DEBUG_MATRIX_PRINT // Matrix class will include a debug 'print' method to output to stdout

//    #define DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE // prevents use of SDL's relative mouse mode which grabs mouse control.
    

//    #define DUA_OLD_VIDEO_DRIVERS // openGL 3.0 will be used instead of 3.3

    #define DUA_FULLSCREEN // engine will run in fullscreen mode

    #define DUA_DEFUALT_WHICH_MONITOR 1
    
    #define DUA_DEFAULT_SCREENRES_X 1024    // Defualt screen resolution X
    #define DUA_DEFAULT_SCREENRES_Y 768     // Defualt screen resolution Y

    #define DUA_DEFAULT_CONSOLEWIDTH 800        // Defualt in-game console width (should be resized to fit screen anyway)
    #define DUA_DEFAULT_CONSOLEHEIGHT 600       // Defualt in-game console height (should be resized to fit screen anyway)

    #define DUA_DEFAULT_SKYMAP "stars"   // Default skymap file name
    #define DUA_DEFAULT_SKYFILETYPE "png"       // Default skymap file format

    #define DUA_VERSION "v0.01.04"  // This is checked against scripts when they are run.

    #define DUA_CONTROLSS_LINEAR_BRAKING_DEADZONE 1e-20
    #define DUA_CONTROLSS_ANGULAR_BRAKING_DEADZONE 1e-20
    #define DUA_CONTROLSS_ANGULAR_BRAKING_ENGAGE_COUNT 20
    #define DUA_CONTROLSS_ANGULAR_BRAKING_THROTTLE 0.25f
    #define DUA_CONTROLSS_ANGULAR_SENSE_MAX 10.f
    #define DUA_CONTROLSS_ANGULAR_SENSE_MIN 0.1f
    
    /* ====================================================================== */
    
    
    /* TYPES */    
    typedef uint_fast32_t   DUA_id;          // used for entity UIDs
    typedef uint_fast16_t   DUA_assetKey;    // used for accessing assets from hash tables
    typedef uint_fast16_t   DUA_compFlag;    // flags used to store which components an entity has
    typedef uint_fast8_t    DUA_stateFlag;   // flags used to store an entity's boolean states
    typedef uint_fast8_t    DUA_compTypeRef; // used to enumerate the different types of components for array access
    typedef uint_fast8_t    DUA_colorByte;   // wrapper for an 8-bit value used for color
    typedef int_fast16_t    DUA_scoreNum;    // wrapper for an int used to keep score
    typedef uint8_t         DUA_uint8;       // three uint types used when buffering to GPU and actual memory size matters.
    typedef uint16_t        DUA_uint16;
    typedef uint32_t        DUA_uint32;
    typedef uint64_t        DUA_uint64;
    typedef float           DUA_float;       // wrapper for the 64 bitfloating point values used by the game
    typedef double          DUA_dbl;         // wrapper for the 32 bit floating point values used by the game
    typedef double          DUA_matrixVal;   // type used for matrix/vector values

    /* ENUMERATORS */    
    /*****************************************
     * The soul component will include a uint
     * whose bits are flags that store whether
     * or not the entity has a given component
     *****************************************/
    enum DUA_componentFlags : DUA_compFlag {
        VALID_COMPS     = 0x1,      // If this bit is 0, error code. Must be on for all souls.
        POSITION        = 0x2,      // Bit 1
        ORIENTATION     = 0x4,
        LINVELOC        = 0x8,
        ANGVELOC        = 0x10,
        COLLISION       = 0x20,
        MODEL           = 0x40,
        CONTROLSS       = 0x80,
        LAMBIENT        = 0x100,
        LDIRECT         = 0x200,
        LPOINT          = 0x400,
        SPATCHILD       = 0x800,
        SPATPARENT      = 0x1000,
        OWNER           = 0x2000,
        SCORE           = 0x4000,        
        FREECAM         = 0x8000    // Bit 16
    };
    /*****************************************
     * The soul component will include a uint
     * whose bits are flags that store boolean
     * states, such as whether or not the
     * entity is experiencing a physics collision.
     *****************************************/
    enum DUA_stateFlags : DUA_stateFlag {
        VALID_STATE     = 0x1,      // If this bit is 0, error code. Must be on for all souls.
        ACTIVE          = 0x2,      // Bit 1
        REMOVAL         = 0x4,      // Bit 2
        HASMOVED        = 0x8,      //
        RECALCPROJMAT   = 0x10,
        RECALCVIEWMAT   = 0x20
    };
    
    /* INTERNALLY-USEFUL DEFINES    **DO NOT MODIFY**   */    
    #define DUA_NULL_ID 0                           // used for entity error states
    #define DUA_START_ID 1                          // ID assignment begins with 1
    #define DUA_DEFAULT_STATE VALID_STATE | ACTIVE  // State entities start with
    #define DUA_INVALID_STATE 0x0                   // For returning errors
    #define DUA_DEFAULT_COMPONENTS VALID_COMPS      // Components entities start with (none)
    #define DUA_INVALID_COMPONENTS  0x0             // For returning error state    
    
    /* FUNCTIONAL MACROS */
    // This is used when passing buffer offsets to the GPU via openGL
    #define DUA_GL_BUFFER_OFFSET(i) ((char *)NULL + (i))    

    /* SETTING DEPENDENT META PROGRAMMING **DO NOT MODIFY**      */      
    #define DUA_GLVERSION_MAJOR 3
    #ifndef DUA_OLD_VIDEO_DRIVERS
        #define DUA_GLVERSION_MINOR 3
    #else
        #define DUA_GLVERSION_MINOR 0
    #endif

    #ifdef DUA_FULLSCREEN
    #define DUA_SDL_SCREENOPTIONS SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
    #else
    #define DUA_SDL_SCREENOPTIONS SDL_WINDOW_OPENGL
    #endif

    #define DUA_ERR     Settings::Console::errPreText
    #define DUA_ERREND  Settings::Console::errPostText
    #define DUA_TAB     Settings::Console::stdTab

    /* RUN-TIME MUTABLE SETTINGS *AKA GLOBAL VARIABLES* */
    
    namespace Settings {
        
        namespace Systems {
            extern uint32_t systemsPauseTimeout;
        }
        
        namespace Display {
            extern int windowResX, windowResY;
            extern float screenAspectRatio;
            extern int whichMonitor, monitorOffsetX, monitorOffsetY;
			extern bool vSync;
        }
        
        namespace Console {
            extern int locX, locY, width, height, marginX, marginY, spacingX, spacingY;
            extern std::string fontName;
            extern uint32_t panelW, panelH;
            extern float baseLineFromTop, stretchMultW, stretchMultH;
            extern char firstChar, lastChar;
            extern glm::vec3 backColor;
            extern glm::vec3 textColor;
            extern std::string consolePrompt, menuPrompt, errPreText, errPostText, stdTab;
        }

        namespace Font {
            extern std::string assetDirPrefix;
            namespace Default {
                extern std::string fontName;
                extern uint32_t panelW, panelH;
                extern float baseLineFromTop, stretchMultW, stretchMultH;
                extern char firstChar, lastChar;
            }
            namespace Sdf {
                namespace Default {
                    extern float fuzzRadiusRatio;
                    extern uint32_t inputW, inputH;
                }
            }
        }

        namespace TextField {
            namespace Default {

            }
        }
        
        namespace Sky {
            extern std::string fileName, fileType;
        }

        namespace ScreenShot {
            extern uint32_t width, height;
        }
    }
    
    namespace Constants {
        extern const glm::mat4 duaIdentMat4;
        extern const double pi;
        extern const double halfPi;
        extern const double quarterPi;
    }
}

#endif	/* DUA_SETTINGS_H */

