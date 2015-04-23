/* 
 * File:   Render.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 2:19 PM
 */

//#ifndef RENDEROLD_H
//#define	RENDEROLD_H

//<editor-fold>
//
//#include "System.h"
//
//#include <SDL.h>
//#include <GL/glew.h>
//#include <SDL_opengl.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//class System_Render : public System
//{
//private:
//    // SDL window
//    SDL_Window* window;
//    // OpenGL context
//    SDL_GLContext context;
//    
//    GLuint vao_ID;
//    GLuint vbo_ID;
//    GLuint simpleShader_ID;
//    GLuint mvpMatrixID;
//    GLuint viewMatrixID;
//    GLuint modelMatrixID;
//    
//    glm::mat4 modelMatrix;
//    glm::mat4 viewMatrix;
//    glm::mat4 projectionMatrix;
//    glm::mat4 mvpMatrix;
//    
//        
//public:
//    System_Render(ComponentBank* bank, SDL_Window* window);
//    ~System_Render();
//    void tick() override;    
//    bool init() override;
//};

//#endif	/* RENDER_H */

//</editor-fold>

////<editor-fold>
//
//// This prevents a ton of compiler warnings
//#define GLM_FORCE_RADIANS
//
//#include <iostream>
//#include <GL/glew.h>
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include "System.h"
//#include "mesh.h"
//#include "lighting_technique.h"
//#include "loadShaders.h"
//
//class System_RenderOld : public System
//{
//private:
//    // SDL window
//    SDL_Window* window = NULL;
//    // OpenGL context
//    SDL_GLContext context;
//    
//    GLuint simpleShader_ID;
//    GLuint fooShader_ID;
//    GLuint debugShader_ID;    
//    
//    int numInstances;
//    Mesh* m_pMesh = NULL;
//    float camPosZ;
//    
//    
//    
//    static const unsigned int MAX_POINT_LIGHTS = 2;
//    static const unsigned int MAX_SPOT_LIGHTS = 2;
//    
//    glm::vec3 camPos;
//    glm::mat4 perspMatrix;
//    DirectionalLightx m_directionalLight;
//    PointLightx m_pointLight;
//    SpotLightx m_spotLight;
//    
//    GLuint m_colorTextureLocation;
//    GLuint m_eyeWorldPosLocation;
//    GLuint m_matSpecularIntensityLocation;
//    GLuint m_matSpecularPowerLocation;
//    GLuint m_numPointLightsLocation;
//    GLuint m_numSpotLightsLocation;
//    GLuint m_colorLocation[4];
//
//    struct {
//        GLuint Color;
//        GLuint AmbientIntensity;
//        GLuint DiffuseIntensity;
//        GLuint Direction;
//    } m_dirLightLocation;
//
//    struct {
//        GLuint Color;
//        GLuint AmbientIntensity;
//        GLuint DiffuseIntensity;
//        GLuint Position;
//        struct {
//            GLuint Constant;
//            GLuint Linear;
//            GLuint Exp;
//        } Atten;
//    } m_pointLightsLocation[MAX_POINT_LIGHTS];
//
//    struct {
//        GLuint Color;
//        GLuint AmbientIntensity;
//        GLuint DiffuseIntensity;
//        GLuint Position;
//        GLuint Direction;
//        GLuint Cutoff;
//        struct {
//            GLuint Constant;
//            GLuint Linear;
//            GLuint Exp;
//        } Atten;
//    } m_spotLightsLocation[MAX_SPOT_LIGHTS];
//    
//    bool setUpLightingEnvironment();
//    void SetColorTextureUnit(unsigned int TextureUnit);
//    void SetDirectionalLight(const DirectionalLightx& Light);
//    void SetPointLights(unsigned int NumLights, const PointLightx* pLights);
//    void SetSpotLights(unsigned int NumLights, const SpotLightx* pLights);
//    void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
//    void SetMatSpecularIntensity(float Intensity);
//    void SetMatSpecularPower(float Power);
//    void SetColor(unsigned int Index, const glm::vec4& Color);
//        
//public:
//    System_RenderOld(ComponentBank* bank, SDL_Window* window);
//    ~System_RenderOld();
//    void tick() override;    
//    bool init() override;
//};
//
//#endif	/* RENDER_H */
//
////</editor-fold> //last used

////<editor-fold>
//
//#include "System.h"
//
//#include <SDL.h>
//#include <GL/glew.h>
//#include <SDL_opengl.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//class System_Render : public System
//{
//private:
//    // SDL window
//    SDL_Window* window = NULL;
//    // OpenGL context
//    SDL_GLContext context;
//    
//    float yRotation = 0;
//        
//public:
//    System_Render(ComponentBank* bank, SDL_Window* window);
//    ~System_Render();
//    void tick() override;    
//    bool init() override;
//    bool initGL();
//};
//
//#endif	/* RENDER_H */
//
////</editor-fold>

