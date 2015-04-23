/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

//#include "../Headers/RenderOld.h"

//<editor-fold>

//System_Render::System_Render(ComponentBank* bank, SDL_Window* window)
//                  : System(bank, "Rendering System")
//{
//    this->window = window;
//}
//
//System_Render::~System_Render()
//{
//    // Cleanup VBO
//    glDeleteBuffers(1, &vbo_ID);
//    glDeleteVertexArrays(1, &vao_ID);
//    glDeleteProgram(simpleShader_ID);
//
//    //Destroy window	
//    SDL_DestroyWindow(window);
//    window = NULL;
//
//    //Quit SDL subsystems
//    SDL_Quit();
//}
//
//bool System_Render::init()
//{  
//    // Create context
//    context = SDL_GL_CreateContext(window);
//    // If the context couldn't be created for whatever reason
//    if(context == NULL)
//    {
//        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
//        return false;
//    }
//    
//    // Initialize GLEW
//    glewExperimental = GL_TRUE; 
//    GLenum glewError = glewInit();
//    if(glewError != GLEW_OK)
//    {
//        printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
//        return false;
//    }
//
//    // Use Vsync
//    if(SDL_GL_SetSwapInterval(1) < 0)
//    {
//        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
//        return false;
//    }
//    
//    // Black background
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//    // Enable depth buffer
//    glEnable(GL_DEPTH_TEST);
//    // Configure depth buffer to render only the closest fragments
//    glDepthFunc(GL_LESS); 
//    // Cull triangle backfaces (only render if face normal points more or less towards camera)
//    glEnable(GL_CULL_FACE);
//    
//    glGenVertexArrays(1, &vao_ID);
//    glBindVertexArray(vao_ID);
//    
//    // Create and compile our GLSL program from the shaders
//    if (simpleShader_ID = loadShaders("Assets/Shaders/debug.vert",
//                                      "Assets/Shaders/debug.frag") == 0)
//        return false;
//    
//    mvpMatrixID = glGetUniformLocation(simpleShader_ID, "MVP");
//    
//    
//    
//    static const GLfloat g_vertex_buffer_data[] =
//    { 
//        -1.0f, -1.0f, 0.0f,
//         1.0f, -1.0f, 0.0f,
//         0.0f,  1.0f, 0.0f,
//    };
//    glGenBuffers(1, &vbo_ID);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//    
//
//    
//    projectionMatrix = glm::perspective(60.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
//    
//    return true;
//}
//
//void System_Render::tick()
//{
//    printf("fu\n");
//    viewMatrix = glm::lookAt
//    (
//        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
//        glm::vec3(0,0,0), // and looks at the origin
//        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
//    );
//
//    modelMatrix = glm::mat4(1.0f);
//
//    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
//    
//    
//    
//    // Clear the screen
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    glUseProgram(simpleShader_ID);
//    
//    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);
//    
//    
//    
//    // 1st attribute buffer : vertices
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
//    glVertexAttribPointer(
//        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
//        3,                  // size
//        GL_FLOAT,           // type
//        GL_FALSE,           // normalized?
//        0,                  // stride
//        (void*)0            // array buffer offset
//    );
//
//    // Draw the triangle !
//    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
//
//    glDisableVertexAttribArray(0);
//    
//    SDL_GL_SwapWindow(window);
//}

//</editor-fold>

////<editor-fold>
//
//System_RenderOld::System_RenderOld(ComponentBank* bank, SDL_Window* window)
//                  : System(bank, "Rendering System")
//{
//    this->window = window;
//    
//    m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
//    m_directionalLight.AmbientIntensity = 0.55f;
//    m_directionalLight.DiffuseIntensity = 0.9f;
//    m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);
//}
//
//System_RenderOld::~System_RenderOld()
//{
//    POINTER_DELETE(m_pMesh);
//    
//    // Cleanup VBO
////    glDeleteBuffers(1, &vbo_ID);
////    glDeleteVertexArrays(1, &vao_ID);
//    glDeleteProgram(simpleShader_ID);
//
//    //Destroy window	
//    SDL_DestroyWindow(window);
//    window = NULL;
//
//    //Quit SDL subsystems
//    SDL_Quit();
//}
//
//bool System_RenderOld::init()
//{
//    Magick::InitializeMagick("");
//    
//    // Create context
//    context = SDL_GL_CreateContext(window);
//    // If the context couldn't be created for whatever reason
//    if(context == NULL)
//    {
//        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
//        return false;
//    }
//    
//    // Initialize GLEW
//    glewExperimental = GL_TRUE; 
//    GLenum glewError = glewInit();
//    if(glewError != GLEW_OK)
//    {
//        printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
//        return false;
//    }
//
//    // Use Vsync
//    if(SDL_GL_SetSwapInterval(1) < 0)
//    {
//        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
//        return false;
//    }
//    
//    // Black background
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//    // Enable depth buffer
//    glEnable(GL_DEPTH_TEST);
//    // Configure depth buffer to render only the closest fragments
//    glDepthFunc(GL_LESS); 
//    // Cull triangle backfaces (only render if face normal points more or less towards camera)
//    glEnable(GL_CULL_FACE);
//    
//    GLenum err = glGetError();
//    if (err != GL_NO_ERROR)
//    {
//        std::cout << "GL error after GL primary init reads: " << err << std::endl << std::endl;
//    }
//    
//    // Create and compile our GLSL program from the shaders
////    if (simpleShader_ID = loadShaders("Assets/Shaders/simple.vert",
////                                      "Assets/Shaders/simple.frag") == 0)
////        return false;
//    if (fooShader_ID = loadShaders("Assets/Shaders/debug.vert",
//                                   "Assets/Shaders/debug.frag") == 0)
//        return false;
////    if (debugShader_ID = loadShaders("Assets/Shaders/debug.vert",
////                                     "Assets/Shaders/debug.frag") == 0)
////        return false;
//    
//    glUseProgram(fooShader_ID);
//
//    m_pMesh = new Mesh();
////    numInstances = 1600;
//    numInstances = 400;
//
//    if (!m_pMesh->LoadMesh("Assets/Models/MetalCube/metalCube.obj")) { //Cereal/
//        return false;            
//    }
//    
//    camPosZ = 0.0;
////    setUpLightingEnvironment();
////    perspMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
//
//    return true;
//}
//
//void System_RenderOld::tick()
//{
//    //std::cout << "renderTick\n";
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);          
//
//    glm::mat4 WVPMatrics[numInstances];
//    glm::mat4 WorldMatrices[numInstances];
//    perspMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
//    
////    camPos = glm::vec3(30, 50, camPosZ);
////    glm::mat4 viewMatrix = glm::lookAt
////    (
////        camPos,
////        glm::vec3(30,30,0),     // looks at the origin
////        glm::vec3(0,1,0)        // Head is up (set to 0,-1,0 to look upside-down)
////    );
//    
//    camPos = glm::vec3(0, 50, camPosZ);
//    glm::mat4 viewMatrix = glm::lookAt
//    (
//        camPos,
//        glm::vec3(0,0,0),     // looks at the origin
//        glm::vec3(0,1,0)        // Head is up (set to 0,-1,0 to look upside-down)
//    );
//    
////    SetEyeWorldPos(camPos);
////    SetDirectionalLight(m_directionalLight);
//    
//    for (unsigned int i = 0 ; i < numInstances ; i++) {
//        
////        WorldMatrices[i] = glm::mat4(1.0);
////        glm::vec3 transVec = glm::vec3((i % 20) * 3.0, ((i / 20) % 20) * 3.0, (i / 400) * -6.0);
////        glm::translate(WorldMatrices[i], transVec);
//        
//        WorldMatrices[i] = glm::mat4(
//                                     1.0,0,0,0,
//                                     0,1.0,0,0,
//                                     0,0,1.0,0,
//                                     (i % 20) * 3.0,((i / 20) % 20) * 3.0,(i / 400) * -6.0,1.0);
//        
//        WVPMatrics[i] = perspMatrix * viewMatrix * WorldMatrices[i];
//    }
//    
//    m_pMesh->Render(numInstances, WVPMatrics, WorldMatrices);
//    
//    //printf("%f\n", camPos.z);
//    
//    camPosZ += .125;
//    
//    SDL_GL_SwapWindow(window);
//}
//
//
//
//
//
//
//bool System_RenderOld::setUpLightingEnvironment()
//{
//    
//    m_colorTextureLocation = glGetUniformLocation(fooShader_ID, "gColorMap");
//    m_eyeWorldPosLocation = glGetUniformLocation(fooShader_ID, "gEyeWorldPos");
//    m_dirLightLocation.Color = glGetUniformLocation(fooShader_ID, "gDirectionalLight.Base.Color");
//    m_dirLightLocation.AmbientIntensity = glGetUniformLocation(fooShader_ID, "gDirectionalLight.Base.AmbientIntensity");
//    m_dirLightLocation.Direction = glGetUniformLocation(fooShader_ID, "gDirectionalLight.Direction");
//    m_dirLightLocation.DiffuseIntensity = glGetUniformLocation(fooShader_ID, "gDirectionalLight.Base.DiffuseIntensity");
//    m_matSpecularIntensityLocation = glGetUniformLocation(fooShader_ID, "gMatSpecularIntensity");
//    m_matSpecularPowerLocation = glGetUniformLocation(fooShader_ID, "gSpecularPower");
//    m_numPointLightsLocation = glGetUniformLocation(fooShader_ID, "gNumPointLights");
//    m_numSpotLightsLocation = glGetUniformLocation(fooShader_ID, "gNumSpotLights");
//    
//    
//
//    if (m_dirLightLocation.AmbientIntensity == INVALID_VALUE_32 ||
//        m_colorTextureLocation == INVALID_VALUE_32 ||
//        m_eyeWorldPosLocation == INVALID_VALUE_32 ||
//        m_dirLightLocation.Color == INVALID_VALUE_32 ||
//        m_dirLightLocation.DiffuseIntensity == INVALID_VALUE_32 ||
//        m_dirLightLocation.Direction == INVALID_VALUE_32 ||
//        m_matSpecularIntensityLocation == INVALID_VALUE_32 ||
//        m_matSpecularPowerLocation == INVALID_VALUE_32 ||
//        m_numPointLightsLocation == INVALID_VALUE_32 ||
//        m_numSpotLightsLocation == INVALID_VALUE_32) {
//        return false;
//    }
//
//    for (unsigned int i = 0 ; i < ARRAY_COUNT(m_pointLightsLocation) ; i++) {
//        char Name[128];
//        memset(Name, 0, sizeof(Name));
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
//        m_pointLightsLocation[i].Color = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
//        m_pointLightsLocation[i].AmbientIntensity = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
//        m_pointLightsLocation[i].Position = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
//        m_pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
//        m_pointLightsLocation[i].Atten.Constant = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
//        m_pointLightsLocation[i].Atten.Linear = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
//        m_pointLightsLocation[i].Atten.Exp = glGetUniformLocation(fooShader_ID, Name);
//
//        if (m_pointLightsLocation[i].Color == INVALID_VALUE_32 ||
//            m_pointLightsLocation[i].AmbientIntensity == INVALID_VALUE_32 ||
//            m_pointLightsLocation[i].Position == INVALID_VALUE_32 ||
//            m_pointLightsLocation[i].DiffuseIntensity == INVALID_VALUE_32 ||
//            m_pointLightsLocation[i].Atten.Constant == INVALID_VALUE_32 ||
//            m_pointLightsLocation[i].Atten.Linear == INVALID_VALUE_32 ||
//            m_pointLightsLocation[i].Atten.Exp == INVALID_VALUE_32) {
//            return false;
//        }
//    }
//
//    for (unsigned int i = 0 ; i < ARRAY_COUNT(m_spotLightsLocation) ; i++) {
//        char Name[128];
//        memset(Name, 0, sizeof(Name));
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
//        m_spotLightsLocation[i].Color = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
//        m_spotLightsLocation[i].AmbientIntensity = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
//        m_spotLightsLocation[i].Position = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
//        m_spotLightsLocation[i].Direction = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
//        m_spotLightsLocation[i].Cutoff = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
//        m_spotLightsLocation[i].DiffuseIntensity = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
//        m_spotLightsLocation[i].Atten.Constant = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
//        m_spotLightsLocation[i].Atten.Linear = glGetUniformLocation(fooShader_ID, Name);
//
//        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
//        m_spotLightsLocation[i].Atten.Exp = glGetUniformLocation(fooShader_ID, Name);
//
//        if (m_spotLightsLocation[i].Color == INVALID_VALUE_32 ||
//            m_spotLightsLocation[i].AmbientIntensity == INVALID_VALUE_32 ||
//            m_spotLightsLocation[i].Position == INVALID_VALUE_32 ||
//            m_spotLightsLocation[i].Direction == INVALID_VALUE_32 ||
//            m_spotLightsLocation[i].Cutoff == INVALID_VALUE_32 ||
//            m_spotLightsLocation[i].DiffuseIntensity == INVALID_VALUE_32 ||
//            m_spotLightsLocation[i].Atten.Constant == INVALID_VALUE_32 ||
//            m_spotLightsLocation[i].Atten.Linear == INVALID_VALUE_32 ||
//            m_spotLightsLocation[i].Atten.Exp == INVALID_VALUE_32) {
//            return false;
//        }
//    }
//
//    for (unsigned int i = 0; i < ARRAY_COUNT(m_colorLocation) ; i++) {
//        char Name[32];
//        memset(Name, 0, sizeof(Name));
//        snprintf(Name, sizeof(Name), "gColor[%d]", i);
//        
//        m_colorLocation[i] = glGetUniformLocation(fooShader_ID, Name);
//        
//        if (m_colorLocation[i] == INVALID_VALUE_32) {
//            return false;            
//        }
//    }
//    
//    
//    
//    SetColorTextureUnit(0);
//    SetDirectionalLight(m_directionalLight);
//    SetMatSpecularIntensity(0.5f);
//    SetMatSpecularPower(1);
//    SetColor(0, glm::vec4(1.0f, 0.5f, 0.5f, 0.0f));
//    SetColor(1, glm::vec4(0.5f, 1.0f, 1.0f, 0.0f));
//    SetColor(2, glm::vec4(1.0f, 0.5f, 1.0f, 0.0f));
//    SetColor(3, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
//    
//    return true;
//}
//
//void System_RenderOld::SetColorTextureUnit(unsigned int TextureUnit)
//{
//    glUniform1i(m_colorTextureLocation, TextureUnit);
//}
//
//
//void System_RenderOld::SetDirectionalLight(const DirectionalLightx& Light)
//{
//    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
//    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
//    glm::vec3 Direction = Light.Direction;
//    glm::normalize(Direction);
//    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
//    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
//}
//
//
//void System_RenderOld::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
//{
//    glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
//}
//
//
//void System_RenderOld::SetMatSpecularIntensity(float Intensity)
//{
//    glUniform1f(m_matSpecularIntensityLocation, Intensity);
//}
//
//
//void System_RenderOld::SetMatSpecularPower(float Power)
//{
//    glUniform1f(m_matSpecularPowerLocation, Power);
//}
//
//
//void System_RenderOld::SetPointLights(unsigned int NumLights, const PointLightx* pLights)
//{
//    glUniform1i(m_numPointLightsLocation, NumLights);
//    
//    for (unsigned int i = 0 ; i < NumLights ; i++) {
//        glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
//        glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
//        glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
//        glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
//        glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
//        glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
//        glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
//    }
//}
//
//void System_RenderOld::SetSpotLights(unsigned int NumLights, const SpotLightx* pLights)
//{
//    glUniform1i(m_numSpotLightsLocation, NumLights);
//
//    for (unsigned int i = 0 ; i < NumLights ; i++) {
//        glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
//        glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
//        glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
//        glUniform3f(m_spotLightsLocation[i].Position,  pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
//        glm::vec3 Direction = pLights[i].Direction;
//        glm::normalize(Direction);
//        glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
//        glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(glm::radians(pLights[i].Cutoff)));
//        glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
//        glUniform1f(m_spotLightsLocation[i].Atten.Linear,   pLights[i].Attenuation.Linear);
//        glUniform1f(m_spotLightsLocation[i].Atten.Exp,      pLights[i].Attenuation.Exp);
//    }
//}
//
//
//void System_RenderOld::SetColor(unsigned int Index, const glm::vec4& Color)
//{
//    glUniform4f(m_colorLocation[Index], Color.x, Color.y, Color.z, Color.w);
//}
//
////</editor-fold> // last used

////<editor-fold>
//
//System_Render::System_Render(ComponentBank* bank, SDL_Window* window)
//                  : System(bank, "Rendering System")
//{
//    this->window = window;
//}
//
//System_Render::~System_Render()
//{
//    //Destroy window	
//    SDL_DestroyWindow(window);
//    window = NULL;
//
//    //Quit SDL subsystems
//    SDL_Quit();
//}
//
//bool System_Render::init()
//{    
//    //Create context
//    context = SDL_GL_CreateContext( window );
//    if( context == NULL )
//    {
//        printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
//        return false;
//    }
//    else
//    {
//        //Use Vsync
//        if( SDL_GL_SetSwapInterval( 1 ) < 0 )
//        {
//            printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
//        }
//
//        //Initialize OpenGL
//        if( !initGL() )
//        {
//            printf( "Unable to initialize OpenGL!\n" );
//            return false;
//        }
//    }
//                        
//    return true;
//}
//
//bool System_Render::initGL()
//{
//    GLenum error = GL_NO_ERROR;
//
//    //Initialize Projection Matrix
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity();
//    gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.1f,100.0f);
//    //Check for error
//    error = glGetError();
//    if( error != GL_NO_ERROR )
//    {
//        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
//        return false;
//    }
//
//    //Initialize Modelview Matrix
//    glMatrixMode( GL_MODELVIEW );
//    
//    //Check for error
//    error = glGetError();
//    if( error != GL_NO_ERROR )
//    {
//        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
//        return false;
//    }
//
//    //Initialize clear color
//    glClearColor( 0.f, 0.f, 0.f, 1.f );
//
//    //Check for error
//    error = glGetError();
//    if( error != GL_NO_ERROR )
//    {
//        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
//        return false;
//    }
//
//    return true;
//}
//
//void System_Render::tick()
//{
//    //Clear color buffer
//    glClear( GL_COLOR_BUFFER_BIT );
//
//    glLoadIdentity();
//    glTranslatef(0.0f,0.0f,-2.0f);
//    glRotatef(yRotation,0.0f,1.0f,0.0f); 
//
//    yRotation += 1;
//
//    glBegin( GL_QUADS );
//            glColor3f(1.f, 0.f, 1.f);
//            glVertex2f( -0.5f, -0.5f );
//            glVertex2f( 0.5f, -0.5f );
//            glColor3f(0.f, 1.f, 1.f);
//            glVertex2f( 0.5f, 0.5f );
//            glVertex2f( -0.5f, 0.5f );
//    glEnd();
//    
//    SDL_GL_SwapWindow(window);
//}
//
////</editor-fold>