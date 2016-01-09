#ifndef DEBUGCUBE_H
#define DEBUGCUBE_H

#include "Settings.h"

namespace DualityEngine {

   /****************************************************************************
    * DEBUG CUBE
    * This is a hard-coded 1x1x1 cube model to debug asset loading and the
    * rendering system in general.
    ***************************************************************************/
    class DebugCube{
    private:
        static const DUA_float vertices[];
        static const DUA_float normals[];
        static const glm::vec2 faces[][4];
        static const DUA_float uvCoords[];
        static const DUA_uint8 indices[];
        static const DUA_colorByte rawTexture[];

        GLuint VAOloc;
        GLuint buffers[4];
        GLuint texture;
        GLuint shdrLoc;
        GLuint attrLoc_verts;
        GLuint attrLoc_norms;
        GLuint attrLoc_uvCoo;
        GLuint unifLoc_MVP;
        GLuint unifLoc_M;
        GLuint txtrLoc_diffuse;
        GLuint unifLoc_lightDir;
        GLuint unifLoc_lightClr;
        bool hasInitialized;

        glm::vec3 lightDir;
        glm::vec3 lightColor;
        glm::mat4 mvp;
        
    public:
        
        DebugCube();
        ~DebugCube();
        bool Init(std::stringstream& output);
        void render(const glm::mat4& m, const glm::mat4& vp);
    };
}

#endif
