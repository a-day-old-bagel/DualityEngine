#include "ShaderRepository.h"

using namespace DualityEngine;

void ShaderRepository::storeShaderProgram(const char* name,
                                          const char* vertFile,
                                          const char* fragFile,
                                          std::stringstream& engineOut)
{
    shaderProgramBank.emplace(name, loadShaders(vertFile, fragFile, engineOut));
}

GLuint ShaderRepository::getShaderProgram(const char* name)
{
    return shaderProgramBank[name];
}
