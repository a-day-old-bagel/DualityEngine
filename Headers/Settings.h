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
    #define POINTER_DELETE(p) if (p) { delete p; p = NULL; }
    #define ARRAY_COUNT(p) (sizeof(p)/sizeof(p[0]))
    #define WRITE_ZEROS(a) memset(a, 0, sizeof(a))
    #define INVALID_VALUE_32 0xFFFFFFFF

    // These define the types we'll be using
    #define IDNUM           uint_fast32_t
    #define REPOKEYNUM      uint_fast16_t
    #define COMPFLAG        uint_fast16_t
    #define STATEFLAG       uint_fast8_t
    #define COLORBYTE       uint_fast8_t
    #define SCORENUM        int_fast16_t
    #define FLOAT           double

    #define NULL_ID 0
    #define START_ID 1

    /*****************************************
     * The state component will include an int
     * whose bits are flags that store whether
     * or not the entity has a given component
     *****************************************/
    const COMPFLAG defaultComponents = 0;
    enum componentFlags : COMPFLAG
    {
        SPATIAL         = 0x1,      // Bit 1
        MOTION          = 0x2,      // Bit 2
        COLLISION       = 0x4,      // Bit 3 ...
        MODEL           = 0x8,
        CONTROL         = 0x10,
        LAMBIENT        = 0x20,
        LDIRECT         = 0x40,
        LPOINT          = 0x80,
    };

    /*****************************************
     * The state component will include an int
     * whose bits are flags that store boolean
     * states, such as whether or not the
     * entity is experiencing a collision.
     *****************************************/
    const STATEFLAG defaultState = 0;
    enum stateFlags : STATEFLAG
    {
        ACTIVE          = 0x1,      // Bit 1
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

