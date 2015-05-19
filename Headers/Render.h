/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#ifndef RENDER_H
#define	RENDER_H

// This prevents a ton of compiler warnings
#define GLM_FORCE_RADIANS

//<editor-fold defaultstate="collapsed" desc="Includes">
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "ShaderRepository.h"
#include "MeshRepository.h"
#include "TextureRepository.h"
#include "../Assets/Models/DebugCube/DebugCube.h"
//</editor-fold>

namespace DualityEngine {

    class System_Render : public System
    {
    private:

        SDL_Window* window = NULL;
        SDL_GLContext context;
        
        DebugCube debugCube;
        
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 vp = glm::mat4(1.0f);

        bool setUpEnvironment(std::stringstream& engineOut);
        bool setUpResources(std::stringstream& engineOut);        

    public:
        System_Render(ComponentBank* bank);
        ~System_Render();
        void tick() override;    
        bool init(std::stringstream& output) override;
        void discoverID(const DUA_ID& ID) override;
        void scrutinizeID(const DUA_ID& ID) override;
    };

}

#endif


