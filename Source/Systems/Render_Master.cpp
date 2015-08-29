/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "Render_Master.h"

using namespace DualityEngine;

System_Render_Master::System_Render_Master(ComponentBank* bank)
                  : System(bank, "Master Rendering System", 0) 
{
    
}

System_Render_Master::~System_Render_Master()
{
    //Destroy window	
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool System_Render_Master::init(std::stringstream& engineOut){

    // make sure SDL's video components are running...
    while(!SDL_WasInit(SDL_INIT_VIDEO)){
        // This loops until it is detected that SDL's video component has initialized.
        // This initialization is performed in the thread containing the userInput system
        // for the sake of the handling of SDL's events occurring in that thread.
        // SDL used to initialize in this thread but that caused slowdowns in the other thread,
        // while initializing in the other thread appears to have no ill effects on this thread.
    }
    
    // struct for getting current display mode.
    SDL_DisplayMode display;
    
    // This block of stuff figures out which monitor to put the window on (or use as fullscreen display).
    // rendering across multiple monitors in fullscreen is something I have not yet investigated.
    int monitorUsed;
    for (monitorUsed = 0; monitorUsed <= Settings::Display::whichMonitor; ++monitorUsed){
        if (SDL_GetCurrentDisplayMode(monitorUsed, &display)){
            engineOut << "<!>    Could not get display mode for monitor " << monitorUsed << ": " << SDL_GetError() << std::endl;
            if (monitorUsed > 0){
                engineOut << "Attempting to use display " << monitorUsed - 1 << std::endl;
                --monitorUsed;
                break;
            } else {            
                return false;
            }
        } else {
            engineOut << "Display " << monitorUsed << " reports: " << display.w << "x" << display.h << "@" << display.refresh_rate << std::endl;
            if (monitorUsed == Settings::Display::whichMonitor){
                #ifdef DUA_FULLSCREEN
                    Settings::Display::screenResX = display.w;
                    Settings::Display::screenResY = display.h;
                #endif
                Settings::Display::screenAspectRatio = (float) Settings::Display::screenResX / (float) Settings::Display::screenResY;
                engineOut << "Running on display " << monitorUsed << std::endl;
            } else {
                Settings::Display::monitorOffsetX += display.w;
            }
        }
    }
    
    Settings::Console::width = Settings::Display::screenResX;
    Settings::Console::height = Settings::Display::screenResY / 2;
    
    // Specify OpenGL version and profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DUA_GLVERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, DUA_GLVERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create the SDL window
    pWindow = SDL_CreateWindow("Duality Engine",
                    Settings::Display::monitorOffsetX + int((display.w - Settings::Display::screenResX) * 0.5),
                    Settings::Display::monitorOffsetY + int((display.h - Settings::Display::screenResY) * 0.5),
                    Settings::Display::screenResX, Settings::Display::screenResY,
                    DUA_SDL_SCREENOPTIONS);    
    // If the window couldn't be created for whatever reason
    if (pWindow == NULL) {
        engineOut << "<!>    SDL window was not created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Create context
    context = SDL_GL_CreateContext(pWindow);
    if(context == NULL) {
        engineOut << "<!>    OpenGL context was not created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Initialize GLEW (openGL Extensions Wrangler)
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit(); // GL enumerator error is thrown here when using openGL versions 3.2+ It's fine.
    if(glewError != GLEW_OK) {
        engineOut << "<!>    Could not initialize GLEW! " << glewGetErrorString(glewError) << std::endl;
        return false;
    }

    //Use Vsync
    if(SDL_GL_SetSwapInterval(1) < 0) {
        engineOut << "<!>    Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
        // Do not return
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); 
    glEnable(GL_CULL_FACE);
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLenum glErr = glGetError();
    if (glErr != GL_NO_ERROR) {
        engineOut << "<!>    glError detected after system init: " << gluErrorString(glErr) << std::endl;
    }
    return true;
}

void System_Render_Master::tick()
{
    //...rendering to framebuffer is finished already in other systems...
    SDL_GL_SwapWindow( pWindow );    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //...rendering to framebuffer starts again in other systems after this...
    
    // update view just once for all rendering systems this frame.
    bank->updateActiveCamera();
}

void System_Render_Master::setMousePosition(int x, int y){
    SDL_WarpMouseInWindow(pWindow, x, y);
}