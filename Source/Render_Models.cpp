/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render_Models.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Constructor">
System_Render_Models::System_Render_Models(ComponentBank* bank)
                  : System(bank, "Model Rendering System", 1) 
{
    requiredComponents.at(0) = MODEL | POSITION | ROTATION;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Destructor">
System_Render_Models::~System_Render_Models()
{
    
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Init">
bool System_Render_Models::init(std::stringstream& engineOut)
{    
    // Load graphics assets and buffer them to GPU
    if(!setUpResources(engineOut)) {
        engineOut << "Unable to initialize graphics resources!" << std::endl;
        return false;
    }
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Set Up Resources">

bool System_Render_Models::setUpResources(std::stringstream& engineOut)
{
    bool success = true;
    
    success &= debugCube.Init(engineOut);
    
    projection = glm::perspective(DUA_DEFAULT_FOV, Settings::screenAspectRatio, DUA_DEFAULT_ZPLANENEAR, DUA_DEFAULT_ZPLANEFAR); //needs to be camera
    view = glm::lookAt(
             glm::vec3(-2, 0, 2),    // Camera position
             glm::vec3(0, 0, 0),    // Camera look-at
             glm::vec3(0, -1, 0));   // "Up" direction\
             
    return success;
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Tick">
void System_Render_Models::tick()
{    
    vp = projection * view;
    
    debugCube.render(vp);
}
//</editor-fold>