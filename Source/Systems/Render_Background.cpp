/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "Render_Background.h"
#include "Render_Master.h"

using namespace DualityEngine;

System_Render_Background::System_Render_Background(ComponentBank* bank)
                  : System<System_Render_Background>(bank, "Sky Rendering System", 0)
{
    queuedFileName = NULL;
    queuedFileType = NULL;
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
	System_Render_Master::checkError(engineOut, "Render_Background.cpp", __LINE__);
    return true;
}

bool System_Render_Background::setUpResources(std::stringstream& engineOut)
{
    bool success = true;

    DUA_float corners[24] = {-1.0, -1.0, 0.5, 1.0,
                             3.0, -1.0, 0.5, 1.0,
                             -1.0,  3.0, 0.5, 1.0};
    
    shdrLoc = loadShaders("Assets/Shaders/skyQuad.vert", "Assets/Shaders/skyQuad.frag", engineOut);

    attrLoc_verts = glGetAttribLocation(shdrLoc, "Vertex");
    unifLoc_txtur = glGetUniformLocation(shdrLoc, "cubeTexture");
    unifLoc_projM = glGetUniformLocation(shdrLoc, "projMat");
    unifLoc_viewM = glGetUniformLocation(shdrLoc, "viewMat");
    
    // buffer part    
    glGenVertexArrays(1, &VAOloc);
    glBindVertexArray(VAOloc);

    glGenBuffers(1, &vertices);

    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DUA_float) * 24, corners, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrLoc_verts);
    glVertexAttribPointer(attrLoc_verts, 4, GL_FLOAT, GL_FALSE, 0, 0);    
    
    // generate texture
    glGenTextures (1, &texture);    

    // load as a cube map
    success &= useCubeMap(Settings::Sky::fileName, Settings::Sky::fileType, engineOut);
    
    // format cube map texture
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
             
    return success;
}

void System_Render_Background::queueSkyChange(std::string& fileName, std::string& fileType){
    queuedFileName = new std::string(fileName);
    queuedFileType = new std::string(fileType);
}

bool System_Render_Background::useCubeMap(std::string& fileName, std::string& fileType){
    return readAndBufferCubeMap (
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negz." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posz." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posy." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negy." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negx." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posx." + fileType).c_str(),
            &texture, bank->dlgt->outputStr);
}

bool System_Render_Background::useCubeMap(std::string& fileName, std::string& fileType, std::stringstream& engineOut){
    return readAndBufferCubeMap (
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negz." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posz." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posy." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negy." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/negx." + fileType).c_str(),
            std::string("Assets/Textures/cubeMaps/" + fileName + "/posx." + fileType).c_str(),
            &texture, engineOut);
}

void System_Render_Background::tick()
{    
    glDepthMask (GL_FALSE);
    if (bank->activeFreeCameraID != DUA_NULL_ID){
        
        glActiveTexture (GL_TEXTURE2);
        glBindTexture (GL_TEXTURE_CUBE_MAP, texture);

        glUseProgram (shdrLoc);
        glUniformMatrix4fv (unifLoc_projM, 1, GL_FALSE, &(bank->pFreeCameraCurrent->projection)[0][0]);
        glUniformMatrix4fv (unifLoc_viewM, 1, GL_FALSE, &(bank->pFreeCameraCurrent->view)[0][0]);
        glUniform1i (unifLoc_txtur, 2);
        glBindVertexArray (VAOloc);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    
    } else {
        noCamBackground.render();
    }
    glDepthMask (GL_TRUE);
    
    if (queuedFileName){
        if (queuedFileType){
            useCubeMap(*queuedFileName, *queuedFileType);
            
            delete queuedFileType;            
            queuedFileType = NULL;
        }
        delete queuedFileName;
        queuedFileName = NULL;
    }
}