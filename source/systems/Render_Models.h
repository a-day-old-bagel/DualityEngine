/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#ifndef RENDERMODELS_H
#define	RENDERMODELS_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "System.h"
#include "repos/MeshRepo.h"

namespace DualityEngine {

    class System_Render_Models : public System<System_Render_Models>
    {
    private:
		MeshRepo repo;
		GLuint shdrLoc;
		GLuint unifLoc_MVP;
		GLuint unifLoc_M;

    public:
        System_Render_Models(Bank * bank);
        ~System_Render_Models();
        void tick();
        bool init(std::stringstream& output);
        void forceRemoveComp(const DUA_id& ID, const DUA_compFlag&);
    };

}

#endif


