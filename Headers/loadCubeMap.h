/* 
 * File:   loadCubeMap.h
 * Author: adayoldbagel
 *
 */

#include <sstream>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>


#ifndef LOADCUBEMAP_H
#define	LOADCUBEMAP_H

namespace DualityEngine{

    bool loadCubeMap (const char* front,
                      const char* back,
                      const char* top,
                      const char* bottom,
                      const char* left,
                      const char* right,
                      GLuint* tex_cube,
                      std::stringstream& engineOut);

    bool loadCubeMapSide (GLuint texture, GLenum side_target, const char* file_name, std::stringstream& engineOut);

}

#endif	/* LOADCUBEMAP_H */

