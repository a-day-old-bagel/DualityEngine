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
    typedef uint_fast32_t   DUA_ID;          // used for entity UIDs
    typedef uint_fast16_t   DUA_ASSETKEY;    // used for accessing assets from hash tables
    typedef uint_fast16_t   DUA_COMPFLAG;    // flags used to store which components an entity has
    typedef uint_fast8_t    DUA_STATEFLAG;   // flags used to store an entity's boolean states
    typedef uint_fast8_t    DUA_COMPTYPEREF; // used to enumerate the different types of components for array access
    typedef uint_fast8_t    DUA_COLORBYTE;   // wrapper for an 8-bit value used for color
    typedef int_fast16_t    DUA_SCORENUM;    // wrapper for an int used to keep score
    typedef uint8_t         DUA_UINT8;       // used for specifying mesh face vertex indices for objects
    typedef uint16_t        DUA_UINT16;
    typedef float           DUA_FLOAT;       // wrapper for the 64 bitfloating point values used by the game
    typedef double          DUA_DBL;         // wrapper for the 32 bit floating point values used by the game
    typedef glm::vec3       DUA_VEC3;        // wrapper for a 3D vector
    typedef glm::vec2       DUA_VEC2;        // wrapper for a 2D vector

    // Enumerators
    
//    /*****************************************
//     * the component hash maps will be kept all
//     * in an array at these indices
//     *****************************************/
//    enum DUA_componentCollectionsArrayIndices : DUA_COMPTYPEREF
//    {
//        SOUL_COLLECTION             = 0,
//        MODEL_COLLECTION            = 1,
//        POSITION_COLLECTION         = 2,
//        ROTATION_COLLECTION         = 3,
//        LINVELOC_COLLECTION         = 4,
//        ANGVELOC_COLLECTION         = 5,
//        COLLISION_COLLECTION        = 6,
//        CONTROL_COLLECTION          = 7,
//        LAMBIENT_COLLECTION         = 8,
//        LDIRECT_COLLECTION          = 9,
//        LPOINT_COLLECTION           = 10,
//        POSCHILD_COLLECTION         = 11,
//        POSPARENT_COLLECTION        = 12,
//        OWNER_COLLECTION            = 13,
//        SCORE_COLLECTION            = 14,
//    };
//    
    /*****************************************
     * The soul component will include a uint
     * whose bits are flags that store whether
     * or not the entity has a given component
     *****************************************/
    enum DUA_componentFlags : DUA_COMPFLAG
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
    enum DUA_stateFlags : DUA_STATEFLAG
    {
        VALID_STATE     = 0x1,      // If this bit is 0, error code. Must be on for all souls.
        INACTIVE        = 0x2,      // Bit 1
        REMOVAL         = 0x4,      // Bit 2
    };
    
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
    #define DUA_SCREENRES_X 800
    #define DUA_SCREENRES_Y 600
    #define DUA_DEFAULTFOV 45.0f
    #define DUA_ASPECTRATIO ((float)DUA_SCREENRES_X / (float)DUA_SCREENRES_Y)
    #define DUA_ZPLANENEAR 0.1
    #define DUA_ZPLANEFAR 100.0
}

#endif	/* DUA_SETTINGS_H */

