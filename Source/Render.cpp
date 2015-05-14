/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Constructor">
System_Render::System_Render(ComponentBank* bank)
                  : System(bank, "Rendering System", 1) 
{
    
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Destructor">
System_Render::~System_Render()
{
    //Destroy window	
    SDL_DestroyWindow(window);
    window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Init">
bool System_Render::init(std::stringstream& engineOut)
{
    requiredComponents.at(0) = MODEL | POSITION | ROTATION ;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf ("SDL did not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Specify OpenGL version and profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Settings::GLmajorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Settings::GLminorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // Create the SDL window
    window = SDL_CreateWindow("Game Engine",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    Settings::screenWidth, Settings::screenHeight,
                    SDL_WINDOW_OPENGL);
    
    // If the window couldn't be created for whatever reason
    if (window == NULL) {
        printf ("SDL window was not created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    //Create context
    context = SDL_GL_CreateContext(window);
    if(context == NULL) {
        engineOut << "OpenGL context was not created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if(!initGL(engineOut)) {
        engineOut << "Unable to initialize graphics!" << std::endl;
        return false;
    }
    //Use Vsync
    if(SDL_GL_SetSwapInterval(1) < 0) {
        engineOut << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
    }    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="InitGL">

bool System_Render::initGL(std::stringstream& engineOut)
{
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK) {
        engineOut << "Could not initialize GLEW! " << glewGetErrorString(glewError) << std::endl;
        return false;
    }
    
    // Set background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Create VAO
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = loadShaders("Assets/Shaders/redTri.vert",
                            "Assets/Shaders/redTri.frag", engineOut);
    
    // If shader compilation / linking didn't work, fail.
    if (programID == GL_FALSE) return false;

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");

    // Projection matrix : field of view, aspect ration, zNear, zFar
    Projection = glm::perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    View = glm::lookAt
            (
                glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
            );
    
    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);
    
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Tick">
void System_Render::tick()
{
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram(programID);

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);

    //Update screen
    SDL_GL_SwapWindow( window );
}
//</editor-fold>