/*************************************************************************
 * This takes file paths for the two shaders, and loads the shaders.
 *************************************************************************/

#include "loadShaders.h"

namespace DualityEngine {

    GLuint loadShaders(const char* vertFilePath, const char* fragFilePath, std::stringstream& engineOut) {
        std::string vertShaderCode;
        std::ifstream vertShaderStream(std::string("assets/shaders/") + vertFilePath, std::ios::in);
        if (vertShaderStream.is_open()) {
            std::string Line = "";
            while (getline(vertShaderStream, Line))
                vertShaderCode += "\n" + Line;
            vertShaderStream.close();
        } else {
            engineOut << DUA_ERR << "Cannot open " << vertFilePath << std::endl << DUA_ERREND;
            return GL_FALSE;
        }

        std::string fragShaderCode;
        std::ifstream fragShaderStream(std::string("assets/shaders/") + fragFilePath, std::ios::in);
        if (fragShaderStream.is_open()) {
            std::string Line = "";
            while (getline(fragShaderStream, Line))
                fragShaderCode += "\n" + Line;
            fragShaderStream.close();
        } else {
            engineOut << DUA_ERR << "Cannot open " << fragFilePath << std::endl << DUA_ERREND;
            return GL_FALSE;
        }

        // These are for validating shaders and checking errors
        GLint result = GL_FALSE;
        int infoLogLength;

        // Compile Vertex Shader
        char const* vertSourcePointer = vertShaderCode.c_str();
        GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShaderID, 1, &vertSourcePointer, NULL);
        glCompileShader(vertShaderID);

        // Check Vertex Shader
        glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            std::vector<char> vertShaderErrorMessage((size_t) infoLogLength);
            glGetShaderInfoLog(vertShaderID, infoLogLength, NULL, &vertShaderErrorMessage[0]);
            engineOut << DUA_ERR << vertFilePath << ": " << &vertShaderErrorMessage[0] << std::endl << DUA_ERREND;
        }
        if (result == GL_FALSE) {
            engineOut << DUA_ERR << vertFilePath << " did not compile!\n" << DUA_ERREND;
            glDeleteShader(vertShaderID);
            return GL_FALSE;
        }

        // Compile Fragment Shader
        char const* fragSourcePointer = fragShaderCode.c_str();
        GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShaderID, 1, &fragSourcePointer, NULL);
        glCompileShader(fragShaderID);

        // Check Fragment Shader
        glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            std::vector<char> fragShaderErrorMessage((size_t) infoLogLength);
            glGetShaderInfoLog(fragShaderID, infoLogLength, NULL, &fragShaderErrorMessage[0]);
            engineOut << DUA_ERR << fragFilePath << ": " << &fragShaderErrorMessage[0] << std::endl << DUA_ERREND;
        }
        if (result == GL_FALSE) {
            engineOut << DUA_ERR << fragFilePath << " did not compile!\n" << DUA_ERREND;
            glDeleteShader(vertShaderID);
            glDeleteShader(fragShaderID);
            return GL_FALSE;
        }

        GLuint progID = glCreateProgram();
        glAttachShader(progID, vertShaderID);
        glAttachShader(progID, fragShaderID);
        glLinkProgram(progID);

        // Check the program
        glGetProgramiv(progID, GL_LINK_STATUS, &result);
        glValidateProgram(progID);
        glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            std::vector<char> progErrorMessage((size_t) infoLogLength);
            glGetProgramInfoLog(progID, infoLogLength, NULL, &progErrorMessage[0]);
            engineOut << DUA_ERR << vertFilePath << ", " << fragFilePath << ": ";
            engineOut << &progErrorMessage[0] << std::endl << DUA_ERREND;

            std::cout << "error (" << infoLogLength << "): "
                      << std::string(progErrorMessage.begin(), progErrorMessage.end()) << std::endl;
        }

        glDeleteShader(vertShaderID);
        glDeleteShader(fragShaderID);

        engineOut << "Shader program [" << vertFilePath << ", " << fragFilePath << "] loaded @" << progID << std::endl;
        return progID;
    }
}


