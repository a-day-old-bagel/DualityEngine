//
// Created by Galen on 3/3/2016.
//

#ifndef DUALITYENGINE_TIMEPIECE_H
#define DUALITYENGINE_TIMEPIECE_H

#include <SDL.h>
#include "settings.h"

namespace DualityEngine {
    class TimePiece {
        DUA_uint32 time;
        DUA_uint32 realTime;
        bool paused;
        void updateTime();
    public:
        TimePiece();
        DUA_uint32 getTime();
        void pause();
        void resume();
    };
}


#endif //DUALITYENGINE_TIMEPIECE_H
