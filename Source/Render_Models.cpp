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
             
    return success;
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Tick">
void System_Render_Models::tick()
{    
    if (bank->activeFreeCameraID != DUA_NULL_ID){
        
        for (auto ID : registeredIDs[0]){
            debugCube.render(bank->getModMat(ID), bank->pFreeCameraCurrent->viewProjection);
        }
    }
}
//</editor-fold>
//bool System_Render_Models::aquireView(){
////    if (bank->activeFreeCameraID != localActiveCamera){
////        localActiveCamera = bank->activeFreeCameraID;
////        if (localActiveCamera != DUA_NULL_ID){
////            pCamCurrent = bank->getCameraFreePtr(localActiveCamera);
////        }
////    }
////    if (localActiveCamera != DUA_NULL_ID){
////        if (bank->getState(localActiveCamera) & RECALCVIEWMAT){            
////            pCamCurrent->updateView(bank->getRotMat(localActiveCamera), bank->getPosMat(localActiveCamera));            
////        }
////        if (bank->getState(localActiveCamera) & RECALCPROJMAT){
////            pCamCurrent->updateProjection();
////        }
////        if (bank->getState(localActiveCamera) & (RECALCVIEWMAT | RECALCPROJMAT)){
////            pCamCurrent->updateViewProjection();
////            bank->stateOff(localActiveCamera, RECALCVIEWMAT | RECALCPROJMAT);
////        }
////        return true;
////    } else {
////        return false;
////    }
//    if (bank->activeFreeCameraID != DUA_NULL_ID){
//        return true;
//    } else {
//        return false;
//    }
//    
//}
void System_Render_Models::clean(){
    System::clean();
//    localActiveCamera = DUA_NULL_ID;
//    pCamCurrent = NULL;
}