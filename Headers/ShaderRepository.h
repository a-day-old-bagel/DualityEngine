/* 
 * File:   shaderRepository.h
 * Author: adayoldbagel
 *
 * Created on February 11, 2015, 12:38 AM
 */

#ifndef SHADERREPOSITORY_H
#define	SHADERREPOSITORY_H

#include <unordered_map>
#include "loadShaders.h"

namespace DualityEngine {

class ShaderRepository
{
private:
    std::unordered_map<std::string, GLuint> shaderProgramBank;
public:
    void storeShaderProgram(const char* name,
                            const char* vertFile, const char* fragFile,
                            std::stringstream& engineOut);
    GLuint getShaderProgram(const char* name);
};

}

#endif	/* SHADERREPOSITORY_H */

