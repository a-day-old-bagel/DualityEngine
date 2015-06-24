/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render_Models.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Constructor">
System_Render_Models::System_Render_Models(ComponentBank* bank)
                  : System(bank, "Model Rendering System", 1) 
{
    requiredComponents.at(0) = MODEL | POSITION | ORIENTATION;
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
    
//    DUA_id camID = bank->createEntity("cam");
//    bank->addPosition(camID, 0, 0, 4);
//    bank->addOrientation(camID, 0, 0, 0);
//    bank->addCameraFree(camID, 1.1, 1.f, 1000.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f); //fov in rads
//    bank->addControl(camID);
//    bank->switchToControl(camID);
//    bank->switchToCam(camID);
//    
//    dbgCube = bank->createEntity("debugCube");
//    bank->addModel(dbgCube, "foo");
//    bank->addPosition(dbgCube, 0, 0, 0);
//    bank->addOrientation(dbgCube, pi, 0, 0);
//    bank->addLinearVeloc(dbgCube, 0, 0, 0);
//    bank->addAngularVeloc(dbgCube, 0, 0.006, 0);
             
    return success;
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Tick">
void System_Render_Models::tick()
{    
    if (aquireView()){
        
        for (auto ID : registeredIDs[0]){
            debugCube.render(bank->getModMat(ID), pCamCurrent->viewProjection);
        }
    }
}
//</editor-fold>
bool System_Render_Models::aquireView(){
    if (bank->activeCamera != localActiveCamera){
        localActiveCamera = bank->activeCamera;
        if (localActiveCamera != DUA_NULL_ID){
            pCamCurrent = bank->getCameraFreePtr(localActiveCamera);
        }
    }
    if (localActiveCamera != DUA_NULL_ID){
        if (bank->getState(localActiveCamera) & RECALCVIEWMAT){            
            pCamCurrent->updateView(bank->getRotMat(localActiveCamera), bank->getPosMat(localActiveCamera));            
        }
        if (bank->getState(localActiveCamera) & RECALCPROJMAT){
            pCamCurrent->updateProjection();
        }
        if (bank->getState(localActiveCamera) & (RECALCVIEWMAT | RECALCPROJMAT)){
            pCamCurrent->updateViewProjection();
            bank->stateOff(localActiveCamera, RECALCVIEWMAT | RECALCPROJMAT);
        }
        return true;
    } else {
        return false;
    }
    
}
void System_Render_Models::clean(){
    System::clean();
    localActiveCamera = DUA_NULL_ID;
    pCamCurrent = NULL;
}