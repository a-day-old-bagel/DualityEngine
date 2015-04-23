/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render.h"

//<editor-fold defaultstate="collapsed" desc="Constructor">
System_Render::System_Render(ComponentBank* bank, SDL_Window* window)
                  : System(bank, "Rendering System") 
{
    this->window = window;
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
bool System_Render::init()
{    
    //Create context
    context = SDL_GL_CreateContext(window);
    if(context == NULL) {
        printf("OpenGL context was not created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    //Initialize OpenGL
    if(!initGL()) {
        printf("Unable to initialize OpenGL!\n");
        return false;
    }
    //Use Vsync
    if(SDL_GL_SetSwapInterval(1) < 0) {
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="InitGL">
bool System_Render::initGL()
{
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK) {
        printf("Could not initialize GLEW! %s\n", glewGetErrorString(glewError));
        return false;
    }
    
    // Set background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Create VAO
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = loadShaders("redTri.vert",
                            "redTri.frag");
    
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
    /*
    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 )
    {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                    quit = true;
            }
            //Handle keypress with current mouse position
            else if( e.type == SDL_TEXTINPUT )
            {
                    int x = 0, y = 0;
                    SDL_GetMouseState( &x, &y );
                    //handleKeys( e.text.text[ 0 ], x, y );
            }
    }
     * */

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