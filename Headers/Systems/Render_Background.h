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
#include "DebugBackground.h"

//</editor-fold>

namespace DualityEngine {

    class System_Render_Background : public System
    {
    private:
                
//        DUA_id localActiveCamera = DUA_NULL_ID;
//        CameraFree* pCamCurrent;
        DebugBackground noCamBackground;
        std::string* queuedFileName = NULL;
        std::string* queuedFileType = NULL;
        
        GLuint vertices;
        GLuint texture;
        GLuint VAOloc;
        GLuint shdrLoc;
        GLint unifLoc_txtur;
        GLint attrLoc_verts;
        GLint unifLoc_projM;
        GLint unifLoc_viewM;

        bool setUpResources(std::stringstream& engineOut);
//        bool aquireView();
        bool useCubeMap(std::string& fileName, std::string& fileType, std::stringstream& engineOut);
        bool useCubeMap(std::string& fileName, std::string& fileType);

    public:
        System_Render_Background(ComponentBank* bank);
        ~System_Render_Background();
        void tick() override;    
        bool init(std::stringstream& output) override;
//        void clean() override;        
        void queueSkyChange(std::string& fileName, std::string& fileType);
    };

}

#endif


