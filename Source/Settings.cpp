#include "../Headers/Settings.h"

namespace DualityEngine {
    namespace Settings{
        int screenResX = DUA_DEFAULT_SCREENRES_X;
        int screenResY = DUA_DEFAULT_SCREENRES_Y;
        float screenAspectRatio = ((float)screenResX / (float)screenResY);
        int whichMonitor = DUA_WHICH_MONITOR;
        int monitorOffsetX = 0;
        int monitorOffsetY = 0;
        int systemsPauseTimeout = 1000; // ms
    }
}
