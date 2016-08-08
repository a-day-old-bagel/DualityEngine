//
// Created by Galen on 8/8/2016.
//

#ifndef DUALITYENGINE_TEXTUREVIEW_H
#define DUALITYENGINE_TEXTUREVIEW_H

#include "FontRepo.h"
#include "CameraFree.h"

namespace DualityEngine {
    class TextureView {
        GLuint buffers[2];
        GLuint vaoLoc;
        GLuint shdrLoc;
        GLint  attrLoc_verts;
        GLint  attrLoc_uvCoo;
        GLint  unifLoc_mvpMx;
        GLint  unifLoc_txtre;

        FontDescriptor* font;

    public:
        bool init(FontDescriptor *loadedFont, std::stringstream &out);
        void draw(CameraFree* camera);
    };
}

#endif //DUALITYENGINE_TEXTUREVIEW_H
