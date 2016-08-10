#include "settings.h"

namespace DualityEngine {
    
    namespace Settings{
        
        namespace Systems{        
            uint32_t systemsPauseTimeout = 5000; // ms
        }
        
        namespace Display{
            int windowResX = DUA_DEFAULT_SCREENRES_X;
            int windowResY = DUA_DEFAULT_SCREENRES_Y;
            float screenAspectRatio = ((float)windowResX / (float)windowResY);
            int whichMonitor = DUA_DEFUALT_WHICH_MONITOR;
            int monitorOffsetX = 0;
            int monitorOffsetY = 0;
			bool vSync = false;
        }
        
        namespace Console{
            int locX = 0;
            int locY = 0;
            int width = DUA_DEFAULT_CONSOLEWIDTH;
            int height = DUA_DEFAULT_CONSOLEHEIGHT;
            int marginX = 10;
            int marginY = 5;
            int spacingX = 2;
            int spacingY = 5;
            std::string fontName = "Inconsolata-LGC.otf";
            uint32_t panelW = 10;
            uint32_t panelH = 20;
            float baseLineFromTop = 0.8;
            float stretchMultW = 1.7f;
            float stretchMultH = 1.f;
            char firstChar = ' ';
            char lastChar = '~';
            glm::vec3 backColor = {0.05, 0.05, 0.05};
            glm::vec3 textColor = {0.5, 1.0, 0.3};
            std::string consolePrompt = ">: ";
            std::string menuPrompt = "MENU>: ";
            std::string errPreText = "<!> ";
            std::string errPostText = "";
            std::string stdTab = "    ";
        }

        namespace Font {
            std::string assetDirPrefix = "assets/fonts/";

            namespace Default {
                std::string fontName = "Inconsolata-LGC.otf";
                uint32_t panelW = 10;
                uint32_t panelH = 20;
                float baseLineFromTop = 0.8;
                float stretchMultW = 1.7f;
                float stretchMultH = 1.f;
                char firstChar = ' ';
                char lastChar = '~';
            }

            namespace Sdf {

                namespace Default {
                    float fuzzRadiusRatio = 0.3f;
                }
            }
        }
        
        namespace Sky{
            std::string fileName = DUA_DEFAULT_SKYMAP;
            std::string fileType = DUA_DEFAULT_SKYFILETYPE;
        }

        namespace ScreenShot {
            uint32_t width = 1920;
            uint32_t height = 1080;
        }
    }
    
    namespace Constants{
        const glm::mat4 duaIdentMat4 = glm::mat4(1.0);
        const double pi = 3.141592653589793;
        const double halfPi = pi * 0.5;
        const double quarterPi = pi * 0.25;
    }
}
