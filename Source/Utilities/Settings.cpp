#include "Settings.h"

namespace DualityEngine {
    
    namespace Settings{
        
        namespace Systems{        
            uint systemsPauseTimeout = 5000; // ms
        }
        
        namespace Display{
            int screenResX = DUA_DEFAULT_SCREENRES_X;
            int screenResY = DUA_DEFAULT_SCREENRES_Y;
            float screenAspectRatio = ((float)screenResX / (float)screenResY);
            int whichMonitor = DUA_DEFUALT_WHICH_MONITOR;
            int monitorOffsetX = 0;
            int monitorOffsetY = 0;
        }
        
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
            glm::vec3 backColor = {0.05, 0.05, 0.05};
            glm::vec3 textColor = {0.5, 1.0, 0.3};
        }
        
        namespace Sky{
            std::string fileName = DUA_DEFAULT_SKYMAP;
            std::string fileType = DUA_DEFAULT_SKYFILETYPE;
        }
    }
    
    namespace Constants{
        const glm::mat4 duaIdentMat4 = glm::mat4(1.0);
        const double pi = 3.141592653589793;
        const double halfPi = pi * 0.5;
        const double quarterPi = pi * 0.25;
    }
}
