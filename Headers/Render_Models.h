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

#include "System.h"
#include "../Assets/Models/DebugCube/DebugCube.h"
//</editor-fold>

namespace DualityEngine {

    class System_Render_Models : public System
    {
    private:
        DebugCube debugCube;
        
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 vp = glm::mat4(1.0f);

        bool setUpResources(std::stringstream& engineOut);        

    public:
        System_Render_Models(ComponentBank* bank);
        ~System_Render_Models();
        void tick() override;    
        bool init(std::stringstream& output) override;
    };

}

#endif


