/*************************************************************************
 * This takes file paths for the two shaders, and loads the shaders.
 *************************************************************************/

#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <GL/glew.h>
#include "settings.h"

namespace DualityEngine {

    GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path, std::stringstream& engineOut);

}

#endif
