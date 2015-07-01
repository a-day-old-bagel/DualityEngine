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

    #define WHICHSKYMAP "Meadow"
    #define WHICHSKYFILETYPE "jpg"
    success &= loadCubeMap (std::string("Assets/Textures/cubeMaps/"  WHICHSKYMAP  "/negz."  WHICHSKYFILETYPE).c_str(),
                            std::string("Assets/Textures/cubeMaps/"  WHICHSKYMAP  "/posz."  WHICHSKYFILETYPE).c_str(),
                            std::string("Assets/Textures/cubeMaps/"  WHICHSKYMAP  "/posy."  WHICHSKYFILETYPE).c_str(),
                            std::string("Assets/Textures/cubeMaps/"  WHICHSKYMAP  "/negy."  WHICHSKYFILETYPE).c_str(),
                            std::string("Assets/Textures/cubeMaps/"  WHICHSKYMAP  "/negx."  WHICHSKYFILETYPE).c_str(),
                            std::string("Assets/Textures/cubeMaps/"  WHICHSKYMAP  "/posx."  WHICHSKYFILETYPE).c_str(),
                            &texture, engineOut);   
             
    return success;
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

        //glDrawElements (GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
        glDrawArrays(GL_QUADS, 0, 4);    
    
    } else {
        noCamBackground.render();
    }
    glDepthMask (GL_TRUE);
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