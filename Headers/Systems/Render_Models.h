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
#include "MeshRepository.h"
#include "DebugCube.h"
//</editor-fold>

namespace DualityEngine {

    class System_Render_Models : public System<System_Render_Models>
    {
    private:

		MeshRepository repo;
        DebugCube debugCube;


		DUA_id testBoxId;
		GLuint shdrLoc;
		GLuint attrLoc_verts;
		GLuint attrLoc_norms;
		GLuint attrLoc_uvCoo;
		GLuint unifLoc_MVP;
		GLuint unifLoc_M;


        bool setUpResources(std::stringstream& engineOut);

    public:
        System_Render_Models(ComponentBank* bank);
        ~System_Render_Models();
        void tick();
        bool init(std::stringstream& output);
    };

}

#endif


