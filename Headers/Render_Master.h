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
//</editor-fold>

namespace DualityEngine {

    class System_Render_Master : public System
    {
    private:

        SDL_Window* pWindow = NULL;
        SDL_GLContext context;

    public:
        System_Render_Master(ComponentBank* bank);
        ~System_Render_Master();
        void tick() override;    
        bool init(std::stringstream& output) override;
        void setMousePosition(int x, int y);
    };

}

#endif


