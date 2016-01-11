/*************************************************************************
 * This takes file paths for the two shaders, and loads the shaders.
 *************************************************************************/

#include "loadShaders.h"

using namespace DualityEngine;

GLuint DualityEngine::loadShaders(const char * vertFilePath,const char * fragFilePath, std::stringstream& engineOut)
{
    engineOut << std::endl; // Just some spacing for console output.
    
    // Create the empty shaders
    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint progID = glCreateProgram();
    
    // These are for validating shaders and checking errors
    GLint result = GL_FALSE;
    int infoLogLength;
    
    std::string vertShaderCode;
    std::ifstream vertShaderStream(vertFilePath, std::ios::in);
    if(vertShaderStream.is_open()) {
        std::string Line = "";
        while(getline(vertShaderStream, Line))
                vertShaderCode += "\n" + Line;
        vertShaderStream.close();
    } else {
        engineOut << "\n<!>    Cannot open " << vertFilePath << std::endl;
        return GL_FALSE;
    }

    std::string fragShaderCode;
    std::ifstream fragShaderStream(fragFilePath, std::ios::in);
    if(fragShaderStream.is_open()) {
        std::string Line = "";
        while(getline(fragShaderStream, Line))
                fragShaderCode += "\n" + Line;
        fragShaderStream.close();
    } else {
        engineOut << "\n<!>    Cannot open " << fragFilePath << std::endl;
        return GL_FALSE;
    }
    
    // Compile Vertex Shader
    engineOut << "Compiling vertex shader : " << vertFilePath;
    char const * vertSourcePointer = vertShaderCode.c_str();
    glShaderSource(vertShaderID, 1, &vertSourcePointer , NULL);
    glCompileShader(vertShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> vertShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertShaderID, infoLogLength, NULL, &vertShaderErrorMessage[0]);
        engineOut << std::endl << &vertShaderErrorMessage[0];
    }
    if (result == GL_FALSE) {
        engineOut << vertFilePath << "\n<!>    VERTEX SHADER DID NOT COMPILE!\n\n";
		glDeleteShader(vertShaderID);
        return GL_FALSE;
    } else {
        engineOut << vertFilePath << " compiled successfully.\n\n";
    }

    // Compile Fragment Shader
    engineOut << "Compiling fragment shader : " << fragFilePath;
    char const * fragSourcePointer = fragShaderCode.c_str();
    glShaderSource(fragShaderID, 1, &fragSourcePointer , NULL);
    glCompileShader(fragShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> fragShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragShaderID, infoLogLength, NULL, &fragShaderErrorMessage[0]);
        engineOut << std::endl << &fragShaderErrorMessage[0];
    }
    if (result == GL_FALSE) {
        engineOut << fragFilePath << "\n<!>    FRAGMENT SHADER DID NOT COMPILE!\n\n";
		glDeleteShader(vertShaderID);
		glDeleteShader(fragShaderID);
        return GL_FALSE;
    } else {
        engineOut << fragFilePath << " compiled successfully.\n\n";
    }
    
    engineOut << "Linking Program...\n";
    
    glAttachShader(progID, vertShaderID);
    glAttachShader(progID, fragShaderID);
    glLinkProgram(progID);

    // Check the program
    glGetProgramiv(progID, GL_LINK_STATUS, &result);
    glValidateProgram(progID);
    glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> progErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(progID, infoLogLength, NULL, &progErrorMessage[0]);
        engineOut << std::endl << &progErrorMessage[0];
    }

    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    engineOut << "shader program successfully loaded and given ID " << progID << std::endl;
    return progID;
}


