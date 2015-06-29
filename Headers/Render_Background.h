/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#ifndef RENDERBACKGROUND_H
#define	RENDERBACKGROUND_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

//<editor-fold defaultstate="collapsed" desc="Includes">
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/euler_angles.hpp>

#include "System.h"
#include "loadShaders.h"
#include "loadCubeMap.h"
#include "../Assets/GUI/NoCamBackground/DebugBackground.h"

//</editor-fold>

namespace DualityEngine {

    class System_Render_Background : public System
    {
    private:
                
        DUA_id localActiveCamera = DUA_NULL_ID;
        CameraFree* pCamCurrent;
        DebugBackground noCamBackground;
        
        GLuint vertices;
        GLuint texture;
        GLuint VAOloc;
        GLuint shdrLoc;
        GLuint txtrLoc;
        GLuint attrLoc_verts;
        GLuint unifLoc_projM;
        GLuint unifLoc_viewM;

        bool setUpResources(std::stringstream& engineOut);
        bool aquireView();

    public:
        System_Render_Background(ComponentBank* bank);
        ~System_Render_Background();
        void tick() override;    
        bool init(std::stringstream& output) override;
        void clean() override;
    };

}

#endif


