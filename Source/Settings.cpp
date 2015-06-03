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
        
        namespace Console{
            int locX = 0;
            int locY = 0;
            int width = DUA_DEFAULT_CONSOLEWIDTH;
            int height = DUA_DEFAULT_CONSOLEHEIGHT;
            int charW = 10;
            int charH = 20;
            int marginX = 10;
            int marginY = 5;
            int spacingX = 2;
            int spacingY = 5;
            std::string fontName = "Assets/Fonts/Inconsolata-LGC.otf";
        }
    }
}
