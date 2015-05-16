/* 
 * File:   Settings.h
 * Author: adayoldbagel
 *
 * Created on January 29, 2015, 2:18 PM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace DualityEngine {
    
    // These define the types we'll be using
    typedef uint_fast32_t   DUA_ID;          // used for entity UIDs
    typedef uint_fast16_t   DUA_ASSETKEY;    // used for accessing assets from hash tables
    typedef uint_fast16_t   DUA_COMPFLAG;    // flags used to store which components an entity has
    typedef uint_fast8_t    DUA_STATEFLAG;   // flags used to store an entity's boolean states
    typedef uint_fast8_t    DUA_COLORBYTE;   // wrapper for an 8-bit value used for color
    typedef int_fast16_t    DUA_SCORENUM;    // wrapper for an int used to keep score
    typedef uint8_t         DUA_UINT8;       // used for specifying mesh face vertex indices
    typedef float           DUA_FLOAT;       // wrapper for the 64 bitfloating point values used by the game
    typedef double          DUA_DBL;         // wrapper for the 32 bit floating point values used by the game
    typedef glm::vec3       DUA_VEC3;        // wrapper for a 3D vector
    typedef glm::vec2       DUA_VEC2;        // wrapper for a 2D vector

    // Some defines of useful values
    #define DUA_NULL_ID 0                    // used for entity error states
    #define DUA_START_ID 1                   // ID assignment begins with 1
    #define DUA_DEFAULT_STATE 0              // State entities start with
    #define DUA_DEFAULT_COMPONENTS 0         // Components entities start with

    // Configurable settings - will be in separate configuration file in the future
    
    //#define DUA_OLD_VIDEO_DRIVERS // Defining this will cause openGL 3.0 to be used instead of 3.3
    #define DUA_GLVERSION_MAJOR 3
    #ifndef DUA_OLD_VIDEO_DRIVERS
    #define DUA_GLVERSION_MINOR 3
    #else
    #define DUA_GLVERSION_MINOR 0
    #endif
    #define DUA_SCREENRES_X 800
    #define DUA_SCREENRES_Y 600

    // Enumerators
    
    /*****************************************
     * The soul component will include a uint
     * whose bits are flags that store whether
     * or not the entity has a given component
     *****************************************/
    enum DUA_componentFlags : DUA_COMPFLAG
    {
        POSITION        = 0x1,      // Bit 1
        ROTATION        = 0x2,
        LINVELOC        = 0x4,
        ANGVELOC        = 0x8,
        COLLISION       = 0x10,
        MODEL           = 0x20,
        CONTROL         = 0x40,
        LAMBIENT        = 0x80,
        LDIRECT         = 0x100,
        LPOINT          = 0x200,
        POSCHILD        = 0x400,
        POSPARENT       = 0x800,
        OWNER           = 0x1000,
        SCORE           = 0x2000,
        
        BLANK           = 0x4000,
        BLANK2          = 0x8000    // Bit 16
    };

    /*****************************************
     * The soul component will include a uint
     * whose bits are flags that store boolean
     * states, such as whether or not the
     * entity is experiencing a physics collision.
     *****************************************/
    enum DUA_stateFlags : DUA_STATEFLAG
    {
        INACTIVE        = 0x1,      // Bit 1
        REMOVAL         = 0x2,      // Bit 2
    };
}

#endif	/* SETTINGS_H */

