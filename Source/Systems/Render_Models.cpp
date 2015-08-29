/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render_Models.h"

using namespace DualityEngine;

System_Render_Models::System_Render_Models(ComponentBank* bank)
                  : System(bank, "Model Rendering System", 1) 
{
    requiredComponents.at(0) = MODEL | POSITION | ORIENTATION;
}

System_Render_Models::~System_Render_Models()
{
    
}

bool System_Render_Models::init(std::stringstream& engineOut)
{
    // Load graphics assets and buffer them to GPU
    if(!setUpResources(engineOut)) {
        engineOut << "Unable to initialize graphics resources!" << std::endl;
        return false;
    }
    return true;
}

bool System_Render_Models::setUpResources(std::stringstream& engineOut)
{
    bool success = true;
    
    success &= debugCube.Init(engineOut);
             
    return success;
}

void System_Render_Models::tick()
{    
    if (bank->activeFreeCameraID != DUA_NULL_ID){
        
        for (auto ID : registeredIDs[0]){
            debugCube.render(bank->getModMat(ID), bank->pFreeCameraCurrent->viewProjection);
        }
    }
}