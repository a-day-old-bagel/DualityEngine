/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#ifndef RENDER_H
#define	RENDER_H

// This prevents a ton of compiler warnings
#define GLM_FORCE_RADIANS

//<editor-fold defaultstate="collapsed" desc="Includes">
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "ShaderRepository.h"
#include "MeshRepository.h"
#include "TextureRepository.h"
//</editor-fold>

namespace DualityEngine {

    class System_Render : public System
    {
    private:
        //<editor-fold defaultstate="collapsed" desc="Members">

        // Window pointer passed in
        SDL_Window* window = NULL;
        // Context created in this class
        SDL_GLContext context;

        // Store for all shaders
        ShaderRepository shaderRepo;
        // Store for all meshes
        MeshRepository meshRepo;
        // Store for all textures
        TextureRepository textureRepo;

        GLuint VertexArrayID;
        GLuint programID;
        GLuint vertexbuffer;
        SDL_Event e;
        GLuint MatrixID;
        glm::mat4 Projection;
        glm::mat4 View;
        glm::mat4 Model;
        glm::mat4 MVP;
        GLfloat g_vertex_buffer_data[9] = { 
            -0.8, -0.8, 0.0f,
             0.8, -0.8, 0.0f,
             0.0f, 0.8, 0.0f
        };

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Methods">

        bool initGL(std::stringstream& engineOut);

        //</editor-fold>

    public:
        //<editor-fold defaultstate="collapsed" desc="Methods">
        System_Render(ComponentBank* bank, SDL_Window* window);
        ~System_Render();
        void tick() override;    
        bool init(std::stringstream& output) override;
        //</editor-fold>
    };

}

#endif


