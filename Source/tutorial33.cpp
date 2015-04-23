///*
//
//	Copyright 2011 Etay Meiri
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//    Tutorial 33 - Instanced Rendering
//*/
//
//#include <math.h>
//#include <GL/glew.h>
//#include <GL/freeglut.h>
//
////#include "ogldev_engine_common.h"
////#include "ogldev_app.h"
////#include "ogldev_util.h"
////#include "ogldev_pipeline.h"
////#include "ogldev_camera.h"
//#include "lighting_technique.h"
////#include "ogldev_glut_backend.h"
//#include "mesh.h"
//
//#define WINDOW_WIDTH  1280  
//#define WINDOW_HEIGHT 1024
//
//#define NUM_ROWS 50
//#define NUM_COLS 20
//#define NUM_INSTANCES NUM_ROWS * NUM_COLS
//
//
//class Tutorial33 : public ICallbacks, public OgldevApp
//{
//public:
//
//    Tutorial33()
//    {
//        //m_pGameCamera = NULL;
//        m_pEffect = NULL;
//        //m_scale = 0.0f;
//        m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
//        m_directionalLight.AmbientIntensity = 0.55f;
//        m_directionalLight.DiffuseIntensity = 0.9f;
//        m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);
//
//        //m_persProjInfo.FOV = 60.0f;
//        //m_persProjInfo.Height = WINDOW_HEIGHT;
//        //m_persProjInfo.Width = WINDOW_WIDTH;
//        //m_persProjInfo.zNear = 1.0f;
//        //m_persProjInfo.zFar = 100.0f;  
//        
//        m_pMesh = NULL;
//    }
//
//    ~Tutorial33()
//    {
//        POINTER_DELETE(m_pEffect);
//        //POINTER_DELETE(m_pGameCamera);
//        POINTER_DELETE(m_pMesh);
//    }    
//
//    bool Init()
//    {
//        m_pEffect = new LightingTechnique();
//
//        if (!m_pEffect->Init()) {
//            printf("Error initializing the lighting technique\n");
//            return false;
//        }
//
//        m_pEffect->Enable();
//
//        m_pEffect->SetColorTextureUnit(0);
//        m_pEffect->SetDirectionalLight(m_directionalLight);
//        m_pEffect->SetMatSpecularIntensity(0.0f);
//        m_pEffect->SetMatSpecularPower(0);
//        m_pEffect->SetColor(0, glm::vec4(1.0f, 0.5f, 0.5f, 0.0f));
//        m_pEffect->SetColor(1, glm::vec4(0.5f, 1.0f, 1.0f, 0.0f));
//        m_pEffect->SetColor(2, glm::vec4(1.0f, 0.5f, 1.0f, 0.0f));
//        m_pEffect->SetColor(3, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
//
//        m_pMesh = new Mesh();
//
//        if (!m_pMesh->LoadMesh("../Content/spider.obj")) {
//            return false;            
//        }
//        
//        return true;
//    }
//    
//
//    virtual void RenderSceneCB()
//    {
//        //m_scale += 0.005f;
//
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        m_pEffect->Enable();
//        m_pEffect->SetEyeWorldPos({0, 0, 0});              
//
//        glm::mat4 WVPMatrics[NUM_INSTANCES];
//        glm::mat4 WorldMatrices[NUM_INSTANCES];
//        
//        for (unsigned int i = 0 ; i < NUM_INSTANCES ; i++) {
//            WorldMatrices[i] = glm::translate((i * 10) % 100, (i / 10) * 10, (i / 100) * 100);
//            glm::mat4 perspMatrix = glm::perspective(60.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
//            glm::mat4 viewMatrix = glm::lookAt
//            (
//                glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
//                glm::vec3(0,0,0), // and looks at the origin
//                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
//            );
//            WVPMatrics[i] = perspMatrix * viewMatrix * WorldMatrices[i];
//        }
//        
//        m_pMesh->Render(NUM_INSTANCES, WVPMatrics, WorldMatrices);
//    }
//    
//private:
//
//    LightingTechnique* m_pEffect;
//    //Camera* m_pGameCamera;
//    //float m_scale;
//    DirectionalLightx m_directionalLight;
//    Mesh* m_pMesh;
//};
//
//
////int main(int argc, char** argv)
////{
////    Magick::InitializeMagick(*argv);
////    GLUTBackendInit(argc, argv, true, false);
////
////    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 33")) {
////        return 1;
////    }
////    
////    SRANDOM;
////
////    Tutorial33* pApp = new Tutorial33();
////
////    if (!pApp->Init()) {
////        return 1;
////    }
////    
////    pApp->Run();
////
////    delete pApp;
//// 
////    return 0;
////}