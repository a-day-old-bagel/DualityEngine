/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render_Background.h"

using namespace DualityEngine;

System_Render_Background::System_Render_Background(ComponentBank* bank)
                  : System(bank, "Sky Rendering System", 0) 
{
    
}

System_Render_Background::~System_Render_Background()
{
    
}

bool System_Render_Background::init(std::stringstream& engineOut)
{
    // Load graphics assets and buffer them to GPU
    if(!setUpResources(engineOut)) {
        engineOut << "Unable to initialize sky resources!" << std::endl;
        return false;
    }
    if(!noCamBackground.Init(engineOut)){
        engineOut << "Unable to initialize noCam background!" << std::endl;
        return false;
    }
    return true;
}

bool System_Render_Background::setUpResources(std::stringstream& engineOut)
{
    bool success = true;
    
    DUA_float corners[16] = {-1.0, -1.0, 0.5, 1.0,
                              1.0, -1.0, 0.5, 1.0,
                              1.0,  1.0, 0.5, 1.0,
                             -1.0,  1.0, 0.5, 1.0};
    
    shdrLoc = loadShaders("Assets/Shaders/skyQuad.vert", "Assets/Shaders/skyQuad.frag", engineOut);

    attrLoc_verts = glGetAttribLocation(shdrLoc, "Vertex");
    txtrLoc = glGetUniformLocation(shdrLoc, "cubeTexture");
    unifLoc_projM = glGetUniformLocation(shdrLoc, "projMat");
    unifLoc_viewM = glGetUniformLocation(shdrLoc, "viewMat");
    
    // buffer part    
    glGenVertexArrays(1, &VAOloc);
    glBindVertexArray(VAOloc);

    glGenBuffers(1, &vertices);

    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DUA_float) * 16, corners, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrLoc_verts);
    glVertexAttribPointer(attrLoc_verts, 4, GL_FLOAT, GL_FALSE, 0, 0);

    success &= loadCubeMapInitial (
            std::string("Assets/Textures/cubeMaps/" + Settings::Sky::fileName + "/negz." + Settings::Sky::fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + Settings::Sky::fileName + "/posz." + Settings::Sky::fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + Settings::Sky::fileName + "/posy." + Settings::Sky::fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + Settings::Sky::fileName + "/negy." + Settings::Sky::fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + Settings::Sky::fileName + "/negx." + Settings::Sky::fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + Settings::Sky::fileName + "/posx." + Settings::Sky::fileType).c_str(),
            &texture, engineOut);
    
    readAndBufferCubeMap (
            std::string("Assets/Textures/cubeMaps/mathy/negz.png").c_str(),
            std::string("Assets/Textures/cubeMaps/mathy/posz.png").c_str(),
            std::string("Assets/Textures/cubeMaps/mathy/posy.png").c_str(),
            std::string("Assets/Textures/cubeMaps/mathy/negy.png").c_str(),
            std::string("Assets/Textures/cubeMaps/mathy/negx.png").c_str(),
            std::string("Assets/Textures/cubeMaps/mathy/posx.png").c_str(),
            &texture, bank->dlgt->outputStr);
             
    return success;
}

void System_Render_Background::queueSkyChange(std::string& fileName, std::string& fileType){
    queuedFileName = new std::string(fileName);
    queuedFileType = new std::string(fileType);
}

bool System_Render_Background::useCubeMap(std::string& fileName, std::string& fileType){
//    glActiveTexture (GL_TEXTURE2);
//    glBindTexture (GL_TEXTURE_CUBE_MAP, texture);
//    glUseProgram (shdrLoc);
    glBindVertexArray (VAOloc);
    return readAndBufferCubeMap (
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negz." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posz." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posy." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negy." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negx." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posx." + fileType).c_str(),
            &texture, bank->dlgt->outputStr);
}

void System_Render_Background::tick()
{    
    glDepthMask (GL_FALSE);
    if (aquireView()){       
        
        glActiveTexture (GL_TEXTURE2);
        glBindTexture (GL_TEXTURE_CUBE_MAP, texture);

        glUseProgram (shdrLoc);
        glUniformMatrix4fv (unifLoc_projM, 1, GL_FALSE, &(pCamCurrent->projection)[0][0]);
        glUniformMatrix4fv (unifLoc_viewM, 1, GL_FALSE, &(pCamCurrent->view)[0][0]);
        glUniform1i (txtrLoc, 2);
        glBindVertexArray (VAOloc);

        glDrawArrays(GL_QUADS, 0, 4);    
    
    } else {
        noCamBackground.render();
    }
    glDepthMask (GL_TRUE);
    
    if (queuedFileName){
        if (queuedFileType){
//            bank->dlgt->output("foo0");
            useCubeMap(*queuedFileName, *queuedFileType);
//            bank->dlgt->output("foo1");
            delete queuedFileName, queuedFileType;
//            bank->dlgt->output("foo2");
            queuedFileName, queuedFileType = NULL;
//            bank->dlgt->output("foo3");
        }
    }
}

bool System_Render_Background::aquireView(){
    if (bank->activeCameraID != localActiveCamera){
        localActiveCamera = bank->activeCameraID;
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

void System_Render_Background::clean(){
    System::clean();
    localActiveCamera = DUA_NULL_ID;
    pCamCurrent = NULL;
}