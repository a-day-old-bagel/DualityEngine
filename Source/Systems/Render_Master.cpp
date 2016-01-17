/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "Render_Master.h"

using namespace DualityEngine;

System_Render_Master::System_Render_Master(ComponentBank* bank)
                  : System<System_Render_Master>(bank, "Master Rendering System", 0)
{

}

System_Render_Master::~System_Render_Master()
{

}

bool System_Render_Master::init(std::stringstream& engineOut){
    // make sure SDL's video components are running...
    while(!SDL_WasInit(SDL_INIT_VIDEO) || bank->pWindow == NULL){
        // This loops until it is detected that SDL's video component has initialized.
        // This initialization is performed in the thread containing the userInput system
        // for the sake of the handling of SDL's events occurring in that thread.
        // SDL used to initialize in this thread but that caused slowdowns in the other thread,
        // while initializing in the other thread appears to have no ill effects on this thread.
		SDL_Delay(5);
    }

    //Create context
    context = SDL_GL_CreateContext(bank->pWindow);
    if(context == NULL) {
        engineOut << "<!>    OpenGL context was not created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
	}
	else {
		engineOut << "GL context created.\n";
	}

	checkError(engineOut, "after creating GL context");

    //Initialize GLEW (openGL Extensions Wrangler)
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();  // GL enumerator error is thrown here when using openGL versions 3.2+ It's fine.
                                    // see https://www.opengl.org/wiki/OpenGL_Loading_Library
    if(glewError != GLEW_OK) {
        engineOut << "<!>    Could not initialize GLEW! " << glewGetErrorString(glewError) << std::endl;
        return false;
	}
	else {
		engineOut << "GLEW initialized.\n";
	}

	checkError(engineOut, "after initializing GLEW");

    //Use Vsync
	if (Settings::Display::vSync) {
		if (SDL_GL_SetSwapInterval(1) < 0) {
			engineOut << "<!>    Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
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
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	checkError(engineOut, "after GL configuration (at end of System_Render_Master::init()");
	
    return true;
}

/*
 * DO NOT call this function before the gl context has initialized.  It will make an infinite loop.
 * This is because glGetError always returns GL_INVALID_OPERATION before there exists a context.
 */
void DualityEngine::System_Render_Master::checkError(std::stringstream& engineOut, const char* context) {
	GLenum glErr = glGetError();
	if (glErr != GL_NO_ERROR) {
		std::stringstream tempOut;
		tempOut << "<!>    GL error(s) detected " << context << ":\n";
		int loopGuard = 0;
		while (glErr != GL_NO_ERROR) {
			if (++loopGuard <= 10) {
				tempOut << "\t\t\t" << gluErrorString(glErr) << std::endl;
				glErr = glGetError();
			}
			else {
				tempOut << "\t\t\t<!> Suppressing further errors...\n";
				break;
			}
		}
		engineOut << tempOut.str();
	}
}

void System_Render_Master::tick()
{
    //...rendering to framebuffer is finished already in other systems...
    SDL_GL_SwapWindow(bank->pWindow);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //...rendering to framebuffer starts again in other systems after this...
    
    // update view just once for all rendering systems this frame.
    bank->updateActiveCamera();
}