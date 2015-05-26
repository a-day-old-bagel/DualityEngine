/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Constructor">
System_Render::System_Render(ComponentBank* bank, Console* console)
                  : System(bank, "Rendering System", 1) 
{
    pConsole = console;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Destructor">
System_Render::~System_Render()
{
    //Destroy window	
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Init">
bool System_Render::init(std::stringstream& engineOut)
{
    // entities to be rendered must have these components
    requiredComponents.at(0) = MODEL | POSITION | ROTATION ;
    
    // Set up window and context
    if (!setUpEnvironment(engineOut)) return false;
    
    // Load graphics assets and buffer them to GPU
    if(!setUpResources(engineOut)) {
        engineOut << "Unable to initialize graphics resources!" << std::endl;
        return false;
    }
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Set Up Environment">

bool System_Render::setUpEnvironment(std::stringstream& engineOut)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf ("SDL did not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    // Specify OpenGL version and profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DUA_GLVERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, DUA_GLVERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);    
    // Create the SDL window
    pWindow = SDL_CreateWindow("Game Engine",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    DUA_SCREENRES_X, DUA_SCREENRES_Y,
                    SDL_WINDOW_OPENGL);    
    // If the window couldn't be created for whatever reason
    if (pWindow == NULL) {
        printf ("SDL window was not created! SDL Error: %s\n", SDL_GetError());
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

//    
    return true;
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Set Up Resources">

bool System_Render::setUpResources(std::stringstream& engineOut)
{
    bool success = true;
    
    success &= debugCube.Init(engineOut);
    success &= GUI_console.Init(engineOut, pConsole, "Assets/Fonts/Inconsolata-LGC.otf", 0, 0, 1024, 384, 10, 20, 2, 5, 5 ,5);
    
    projection = glm::perspective(DUA_DEFAULTFOV, DUA_ASPECTRATIO, (float)DUA_ZPLANENEAR, (float)DUA_ZPLANEFAR);
    //projection = glm::ortho(0, DUA_SCREENRES_X, 0, DUA_SCREENRES_Y);
    view = glm::lookAt(
             glm::vec3(-2, 0, 2),    // Camera position
             glm::vec3(0, 0, 0),    // Camera look-at
             glm::vec3(0, -1, 0));   // "Up" direction\
             
    return success;
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Tick">
void System_Render::tick()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    vp = projection * view;
    
    debugCube.render(vp);
    GUI_console.render();
    
    SDL_GL_SwapWindow( pWindow );
}
//</editor-fold>
void System_Render::discoverID(const DUA_id& ID){
    System::discoverID(ID);
}
void System_Render::scrutinizeID(const DUA_id& ID){
    System::scrutinizeID(ID);
}