#include "../Headers/ShaderRepository.h"

using namespace DualityEngine;

void ShaderRepository::storeShaderProgram(const char* name,
                                          const char* vertFile,
                                          const char* fragFile)
{
    shaderProgramBank.emplace(name, loadShaders(vertFile, fragFile));
}

GLuint ShaderRepository::getShaderProgram(const char* name)
{
    return shaderProgramBank[name];
}
