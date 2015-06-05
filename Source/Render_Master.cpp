/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render_Master.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Constructor">
System_Render_Master::System_Render_Master(ComponentBank* bank)
                  : System(bank, "Console Rendering System", 0) 
{
    
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Destructor">
System_Render_Master::~System_Render_Master()
{
    //Destroy window	
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Init">
bool System_Render_Master::init(std::stringstream& engineOut)
{    
    // Set up window and context
    if (!setUpEnvironment(engineOut)) return false;
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Set Up Environment">

bool System_Render_Master::setUpEnvironment(std::stringstream& engineOut)
{   
    //SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
    
    // struct for getting current display mode.
    SDL_DisplayMode display;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        engineOut << "SDL did not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    int monitorUsed = 0;
    for (monitorUsed = 0; monitorUsed <= Settings::whichMonitor; monitorUsed++){
        if (SDL_GetCurrentDisplayMode(monitorUsed, &display)){
            engineOut << "Could not get display mode for monitor " << monitorUsed << ": " << SDL_GetError() << std::endl;
            return false;
        } else {
            engineOut << "Display " << monitorUsed << " reports: " << display.w << "x" << display.h << "@" << display.refresh_rate << std::endl;
            if (monitorUsed == Settings::whichMonitor){
                #ifdef DUA_FULLSCREEN
                    Settings::screenResX = display.w;
                    Settings::screenResY = display.h;
                #endif
                Settings::screenAspectRatio = (float) Settings::screenResX / (float) Settings::screenResY;
                engineOut << "Running on display " << monitorUsed << std::endl;
            } else {
                Settings::monitorOffsetX += display.w;
            }
        }
    }
    
    Settings::Console::width = Settings::screenResX;
    Settings::Console::height = Settings::screenResY / 2;
    
    // Specify OpenGL version and profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DUA_GLVERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, DUA_GLVERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);    
    // Create the SDL window
    pWindow = SDL_CreateWindow("Duality Engine",
                    Settings::monitorOffsetX, Settings::monitorOffsetY,
                    Settings::screenResX, Settings::screenResY,
                    DUA_SDL_SCREENOPTIONS);    
    // If the window couldn't be created for whatever reason
    if (pWindow == NULL) {
        engineOut << "SDL window was not created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }    
    //Create context
    context = SDL_GL_CreateContext(pWindow);
    if(context == NULL) {
        engineOut << "OpenGL context was not created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    //Initialize GLEW (openGL Extensions Wrangler)
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK) {
        engineOut << "Could not initialize GLEW! " << glewGetErrorString(glewError) << std::endl;
        return false;
    }    
    //Use Vsync
    if(SDL_GL_SetSwapInterval(1) < 0) {
        engineOut << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); 
    glEnable(GL_CULL_FACE);
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Tick">
void System_Render_Master::tick()
{
    //...rendering to framebuffer is finished already in other systems...
    SDL_GL_SwapWindow( pWindow );    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //...rendering to framebuffer starts again in other systems after this...
}
//</editor-fold>