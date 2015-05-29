/* 
 * File:   Settings.h
 * Author: adayoldbagel
 *
 * Created on January 29, 2015, 2:18 PM
 */

#ifndef DUA_SETTINGS_H
#define	DUA_SETTINGS_H

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace DualityEngine {
    
    // These define the types we'll be using
    typedef uint_fast32_t   DUA_id;          // used for entity UIDs
    typedef uint_fast16_t   DUA_assetKey;    // used for accessing assets from hash tables
    typedef uint_fast16_t   DUA_compFlag;    // flags used to store which components an entity has
    typedef uint_fast8_t    DUA_stateFlag;   // flags used to store an entity's boolean states
    typedef uint_fast8_t    DUA_compTypeRef; // used to enumerate the different types of components for array access
    typedef uint_fast8_t    DUA_colorByte;   // wrapper for an 8-bit value used for color
    typedef int_fast16_t    DUA_scoreNum;    // wrapper for an int used to keep score
    typedef uint8_t         DUA_uint8;       // used for specifying mesh face vertex indices for objects
    typedef uint16_t        DUA_uint16;
    typedef uint32_t        DUA_uint32;
    typedef float           DUA_float;       // wrapper for the 64 bitfloating point values used by the game
    typedef double          DUA_dbl;         // wrapper for the 32 bit floating point values used by the game
    typedef glm::vec3       DUA_vec3;        // wrapper for a 3D vector
    typedef glm::vec2       DUA_vec2;        // wrapper for a 2D vector
    
    /*****************************************
     * The soul component will include a uint
     * whose bits are flags that store whether
     * or not the entity has a given component
     *****************************************/
    enum DUA_componentFlags : DUA_compFlag
    {
        VALID_COMPS     = 0x1,      // If this bit is 0, error code. Must be on for all souls.
        POSITION        = 0x2,      // Bit 1
        ROTATION        = 0x4,
        LINVELOC        = 0x8,
        ANGVELOC        = 0x10,
        COLLISION       = 0x20,
        MODEL           = 0x40,
        CONTROL         = 0x80,
        LAMBIENT        = 0x100,
        LDIRECT         = 0x200,
        LPOINT          = 0x400,
        POSCHILD        = 0x800,
        POSPARENT       = 0x1000,
        OWNER           = 0x2000,
        SCORE           = 0x4000,
        
        BLANK           = 0x8000    // Bit 16
    };

    /*****************************************
     * The soul component will include a uint
     * whose bits are flags that store boolean
     * states, such as whether or not the
     * entity is experiencing a physics collision.
     *****************************************/
    enum DUA_stateFlags : DUA_stateFlag
    {
        VALID_STATE     = 0x1,      // If this bit is 0, error code. Must be on for all souls.
        INACTIVE        = 0x2,      // Bit 1
        REMOVAL         = 0x4,      // Bit 2
    };
    
    // Macros
    #define DUA_GL_BUFFER_OFFSET(i) ((char *)NULL + (i))
    
    // Some defines of useful values
    #define DUA_NULL_ID 0                           // used for entity error states
    #define DUA_START_ID 1                          // ID assignment begins with 1
    #define DUA_DEFAULT_STATE VALID_STATE           // State entities start with
    #define DUA_INVALID_STATE 0x0                   // For returning errors
    #define DUA_DEFAULT_COMPONENTS VALID_COMPS      // Components entities start with (none)
    #define DUA_INVALID_COMPONENTS  0x0             // For returning errors

    // Configurable settings - will be in separate configuration file in the future
    
    #define DUA_OLD_VIDEO_DRIVERS // Defining this will cause openGL 3.0 to be used instead of 3.3
    #define DUA_GLVERSION_MAJOR 3
    #ifndef DUA_OLD_VIDEO_DRIVERS
        #define DUA_GLVERSION_MINOR 3
    #else
        #define DUA_GLVERSION_MINOR 0
    #endif
    #define DUA_SDL_SCREENOPTIONS SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
    #define DUA_SCREENRES_X 1366
    #define DUA_SCREENRES_Y 768
    #define DUA_DEFAULTFOV 45.0f
    #define DUA_ASPECTRATIO ((float)DUA_SCREENRES_X / (float)DUA_SCREENRES_Y)
    #define DUA_ZPLANENEAR 0.1
    #define DUA_ZPLANEFAR 100.0
}

#endif	/* DUA_SETTINGS_H */

