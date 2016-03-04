//
// Created by Galen on 3/3/2016.
//

#include "TimePiece.h"

namespace DualityEngine {
    TimePiece::TimePiece() {
        time = SDL_GetTicks();
        realTime = time;
        paused = false;
    }
    DUA_uint32 DualityEngine::TimePiece::getTime() {
        updateTime();
        return time;
    }
    void TimePiece::pause() {
        paused = true;
    }
    void TimePiece::resume() {
        realTime = SDL_GetTicks();
        paused = false;
    }
    void TimePiece::updateTime() {
        if (!paused) {
            DUA_uint32 sdlTime = SDL_GetTicks();
            time += sdlTime - realTime;
            realTime = sdlTime;
        }
    }
}
