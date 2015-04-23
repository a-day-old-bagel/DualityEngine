/*************************************************************************
 * This takes file paths for the two shaders, and loads the shaders.
 *************************************************************************/

#include "../Headers/loadShaders.h"

GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path)
{
    //<editor-fold defaultstate="collapsed" desc="Set up variables">
    
    printf("\n"); // Just some spacing for console output.
    
    // Create the empty shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint ProgramID = glCreateProgram();
    
    // These are for validating shaders and checking errors
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    //</editor-fold>
    //<editor-fold defaultstate="collapsed" desc="Read the shader source files">
    
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()) {
            std::string Line = "";
            while(getline(VertexShaderStream, Line))
                    VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
    } else {
            printf("Cannot find %s.\n", vertex_file_path);
            return GL_FALSE;
    }

    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()) {
            std::string Line = "";
            while(getline(FragmentShaderStream, Line))
                    FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
    } else {
            printf("Cannot find %s.\n", fragment_file_path);
            return GL_FALSE;
    }
    
    //</editor-fold>
    //<editor-fold defaultstate="collapsed" desc="Compile each shader">
    
    // Compile Vertex Shader
    printf("Compiling vertex shader : %s", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("\n%s", &VertexShaderErrorMessage[0]);
    }
    if (Result == GL_FALSE) {
        printf("%s SHADER DID NOT COMPILE!\n\n", vertex_file_path);
        return GL_FALSE;
    } else {
        printf("%s compiled successfully.\n\n", vertex_file_path);
    }

    // Compile Fragment Shader
    printf("Compiling fragment shader : %s", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("\n%s", &FragmentShaderErrorMessage[0]);
    }
    if (Result == GL_FALSE) {
        printf("%s SHADER DID NOT COMPILE!\n\n", fragment_file_path);
        return GL_FALSE;
    } else {
        printf("%s compiled successfully.\n\n", fragment_file_path);
    }
    
    //</editor-fold>
    //<editor-fold defaultstate="collapsed" desc="Attach and link shaders">
    
    printf("Linking program");
    
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glValidateProgram(ProgramID);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    //</editor-fold>

    printf("shader program successfully loaded and given ID %i\n\n", ProgramID);
    return ProgramID;
}


