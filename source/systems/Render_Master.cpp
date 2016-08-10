/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "Render_Master.h"
#include "errorChecks.h"
#include <ctime>
#include <iomanip>

namespace DualityEngine {

    System_Render_Master::System_Render_Master(Bank* bank)
            : System<System_Render_Master>(bank, "Master Rendering System", 0) { }

    bool System_Render_Master::onInit(std::stringstream& engineOut) {
        // make sure SDL's video components are running...
        while (!SDL_WasInit(SDL_INIT_VIDEO) || bank->pWindow == NULL) {
            // This loops until it is detected that SDL's video component has initialized.
            // This initialization is performed in the thread containing the userInput system
            // for the sake of the handling of SDL's events occurring in that thread.
            // SDL used to initialize in this thread but that caused slowdowns in the other thread,
            // while initializing in the other thread appears to have no ill effects on this thread.
            SDL_Delay(5);
        }

        //Create context
        context = SDL_GL_CreateContext(bank->pWindow);
        if (context == NULL) {
            engineOut << DUA_ERR << "OpenGL context was not created! SDL Error: ";
            engineOut << SDL_GetError() << std::endl << DUA_ERREND;
            return false;
        }
        else {
            engineOut << "GL context created.\n";
        }

        checkError(engineOut, "Render_Master.cpp", __LINE__);

        //Initialize GLEW (openGL Extensions Wrangler)
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();  // GL enumerator error is thrown here when using openGL versions 3.2+ It's fine.
        // see https://www.opengl.org/wiki/OpenGL_Loading_Library
        if (glewError != GLEW_OK) {
            engineOut << DUA_ERR << "Could not initialize GLEW! ";
            engineOut << glewGetErrorString(glewError) << std::endl << DUA_ERREND;
            return false;
        }
        else {
            engineOut << "GLEW initialized.\n";
        }

        checkError(engineOut, "Render_Master.cpp", __LINE__);

        //Use Vsync
        if (Settings::Display::vSync) {
            if (SDL_GL_SetSwapInterval(1) < 0) {
                engineOut << DUA_ERR << "Warning: Unable to set VSync! SDL Error: ";
                engineOut << SDL_GetError() << std::endl << DUA_ERREND;
                // Do not return
            }
            else {
                engineOut << "VSync enabled.\n";
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        checkError(engineOut, "after GL configuration (at end of System_Render_Master::init()");

        screenShotTaker.init(Settings::ScreenShot::width, Settings::ScreenShot::height);

        return true;
    }

    void System_Render_Master::onTick() {
        if (screenShotInProgress) {
            screenShotInProgress = false;
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            std::stringstream fileNameStream;
            fileNameStream << "screenshot" << std::put_time(&tm, "%Y%m%d%H%M%S") << ".png";
            screenShotTaker.writeImageToFile(fileNameStream.str().c_str());
            screenShotTaker.revertScreenShotState();
        } else if (screenShotQueued) {
            screenShotQueued = false;
            screenShotInProgress = true;
            screenShotTaker.injectScreenShotState(*bank);
        } else {
            //...rendering to framebuffer is finished already in other systems...
            SDL_GL_SwapWindow(bank->pWindow);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            //...rendering to framebuffer starts again in other systems after this...
        }
        bank->updateActiveCamera(bank->updateRenderTime()); // Camera updated here for all rendering systems
    }

    void System_Render_Master::onClean() {

    }

    void System_Render_Master::takeScreenShot() {
        screenShotQueued = true;
    }
}
