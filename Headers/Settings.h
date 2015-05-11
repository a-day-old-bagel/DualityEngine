/* 
 * File:   Settings.h
 * Author: adayoldbagel
 *
 * Created on January 29, 2015, 2:18 PM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

namespace DualityEngine {
    
    // Some useful defines
    #define DU_POINTER_DELETE(p) if (p) { delete p; p = NULL; }
    #define DU_ARRAY_COUNT(p) (sizeof(p)/sizeof(p[0]))
    #define DU_WRITE_ZEROS(a) memset(a, 0, sizeof(a))

    // These define the types we'll be using
    typedef uint_fast32_t   DU_ID;
    typedef uint_fast16_t   DU_ASSETKEY;
    typedef uint_fast16_t   DU_COMPFLAG;
    typedef uint_fast8_t    DU_STATEFLAG;
    typedef uint_fast8_t    DU_COLORBYTE;
    typedef int_fast16_t    DU_SCORENUM;
    typedef double          DU_FLOAT;

    // Some defines of useful values
    #define DU_INVALID_VALUE_32 0xFFFFFFFF  // used for error states
    #define DU_NULL_ID 0                    // used for entity error states
    #define DU_START_ID 1                   // ID assignment begins with 1
    #define DU_DEFAULT_STATE 0              // State entities start with
    #define DU_DEFAULT_COMPONENTS 0         // Components entities start with

    /*****************************************
     * The soul component will include an int
     * whose bits are flags that store whether
     * or not the entity has a given component
     *****************************************/
    enum componentFlags : DU_COMPFLAG
    {
        POSITION        = 0x1,      // Bit 1
        ROTATION        = 0x2,
        POSVELOC        = 0x4,
        ROTVELOC        = 0x8,
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
     * The soul component will include an int
     * whose bits are flags that store boolean
     * states, such as whether or not the
     * entity is experiencing a collision.
     *****************************************/
    enum stateFlags : DU_STATEFLAG
    {
        INACTIVE        = 0x1,      // Bit 1
        REMOVAL         = 0x2,      // Bit 2
    };

    class Settings
    {
    public:
        static const int screenWidth        = 800;
        static const int screenHeight       = 600;
        static const int GLmajorVersion     = 3;
        static const int GLminorVersion     = 3;
    };
}

#endif	/* SETTINGS_H */

