/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#ifndef RENDERMASTER_H
#define	RENDERMASTER_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

//<editor-fold defaultstate="collapsed" desc="Includes">
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include "System.h"
#include "ScreenShot.h"
//</editor-fold>

namespace DualityEngine {

    class System_Render_Master : public System<System_Render_Master>
    {
    private:

        SDL_GLContext context;
        ScreenShotTaker screenShotTaker;
        bool screenShotQueued = false;
        bool screenShotInProgress = false;

    public:
        System_Render_Master(Bank * bank);
        void onTick();
        bool onInit(std::stringstream& output);
        void onClean();
        void takeScreenShot();
    };

}

#endif


