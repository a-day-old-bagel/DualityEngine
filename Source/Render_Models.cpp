/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render_Models.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Constructor">
System_Render_Models::System_Render_Models(ComponentBank* bank)
                  : System(bank, "Model Rendering System", 2) 
{
    requiredComponents.at(0) = MODEL | POSITION | ROTATION;
    requiredComponents.at(1) = FREECAM | POSITION | ROTATION;
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
    
    DUA_id camID = bank->createEntity("cam");
    bank->addPosition(camID, 0, 0, 3);
    bank->addRotation(camID, 0, 0, 0);
    bank->addCameraFree(camID, 60.f, 1.f, 100.f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f);
    std::cout << bank->listComponents(camID) << std::endl;
    bank->switchToCam(camID);
    
//    projection = glm::perspective(DUA_DEFAULT_FOV, Settings::screenAspectRatio, DUA_DEFAULT_ZPLANENEAR, DUA_DEFAULT_ZPLANEFAR); //needs to be camera
//    view = glm::lookAt(
//             glm::vec3(-2, 0, 2),    // Camera position
//             glm::vec3(0, 0, 0),    // Camera look-at
//             glm::vec3(0, -1, 0));   // "Up" direction\
             
    return success;
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Tick">
void System_Render_Models::tick()
{    
    if (aquireView()){
        debugCube.render(pRepUseCam->viewProjection);
    }
}
//</editor-fold>
bool System_Render_Models::aquireView(){
    std::cout << bank->activeCamera << std::endl;
    if (bank->activeCamera != localActiveCamera){
        std::cout << "foo1\n";
        localActiveCamera = bank->activeCamera;
        pRepUseCam = bank->getCameraFreePtr(localActiveCamera);
    }
    if (localActiveCamera != DUA_NULL_ID){
        std::cout << "foo2\n";
        if (bank->getState(localActiveCamera) & RECALCVIEWMAT){
            std::cout << "foo3\n";
            repUseCamTransMat = identMat;
            repUseCamRotatMat = identMat;
            repUseRot = {0, 0, 0};
            if (bank->getComponents(localActiveCamera) & POSITION){
                repUseCamTransMat = glm::translate(repUseCamTransMat, bank->getPositionPtr(localActiveCamera)->position);                
            }
            // IF has position parent goes here
            if (bank->getComponents(localActiveCamera) & ROTATION){
                repUseRot += bank->getRotationPtr(localActiveCamera)->rotation;
            }
            // IF has rotation parent goes here
            repUseCamRotatMat = glm::eulerAngleYXZ(repUseRot.y, repUseRot.x, repUseRot.z);
            
            repUseCamModelMat = repUseCamTransMat * repUseCamRotatMat;
            repUseEye = DUA_vec3(repUseCamModelMat * pRepUseCam->eye);
            repUseFocus = DUA_vec3(repUseCamModelMat * pRepUseCam->focus);
            repUseUp = DUA_vec3(repUseCamModelMat * pRepUseCam->up);
            pRepUseCam->view = glm::lookAt(repUseEye, repUseFocus, repUseUp);
        }
        if (bank->getState(localActiveCamera) & RECALCPROJMAT){
            std::cout << "foo4\n";
            pRepUseCam->projection = glm::perspective(pRepUseCam->fov, Settings::screenAspectRatio, pRepUseCam->zNear, pRepUseCam->zFar);
        }
        if (bank->getState(localActiveCamera) & (RECALCVIEWMAT | RECALCPROJMAT)){
            std::cout << "foo5\n";
            pRepUseCam->viewProjection = pRepUseCam->projection * pRepUseCam->view;
            std::cout << repUseEye.x << repUseEye.y << repUseEye.z << " " << repUseFocus.x << repUseFocus.y << repUseFocus.z << std::endl;
        }
        return true;
    } else {
        return false;
    }
    
}