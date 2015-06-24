/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#ifndef RENDERMODELS_H
#define	RENDERMODELS_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

//<editor-fold defaultstate="collapsed" desc="Includes">
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "System.h"
#include "../Assets/Models/DebugCube/DebugCube.h"
//</editor-fold>

namespace DualityEngine {

    class System_Render_Models : public System
    {
    private:
                
        DUA_id localActiveCamera = DUA_NULL_ID;
        CameraFree* pCamCurrent;
       
        DUA_id dbgCube;
        DebugCube debugCube;

        bool setUpResources(std::stringstream& engineOut);
        bool aquireView();

    public:
        System_Render_Models(ComponentBank* bank);
        ~System_Render_Models();
        void tick() override;    
        bool init(std::stringstream& output) override;
        void clean() override;
    };

}

#endif

