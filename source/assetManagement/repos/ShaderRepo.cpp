#include "ShaderRepo.h"

namespace DualityEngine {

    void ShaderRepo::storeShaderProgram(const char* name,
                                        const char* vertFile,
                                        const char* fragFile,
                                        std::stringstream& engineOut) {
        shaderProgramBank.emplace(name, loadShaders(vertFile, fragFile, engineOut));
    }

    GLuint ShaderRepo::getShaderProgram(const char* name) {
        return shaderProgramBank[name];
    }
}
