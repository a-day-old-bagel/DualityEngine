//
// Created by Galen on 8/8/2016.
//

#ifndef DUALITYENGINE_TEXTUREVIEW_H
#define DUALITYENGINE_TEXTUREVIEW_H

#include "FontRepo.h"
#include "CameraFree.h"

namespace DualityEngine {
    class DebugFontView {
        GLuint buffers[2];
        GLuint vaoLoc;
        GLuint shdrLoc;
        GLint  attrLoc_verts;
        GLint  attrLoc_uvCoo;
        GLint  unifLoc_mvpMx;
        GLint  unifLoc_txtre;
        GLint  unifLoc_pxAdvX;
        GLint  unifLoc_pxAdvY;
        GLint  unifLoc_fzzRad;

        FontDescriptor* font;

    public:
        bool init(FontDescriptor *loadedFont, std::stringstream &out, float x, float y, float z);
        void draw(CameraFree* camera);
    };
}

#endif //DUALITYENGINE_TEXTUREVIEW_H
