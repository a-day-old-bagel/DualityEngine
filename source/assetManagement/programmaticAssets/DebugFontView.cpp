//
// Created by Galen on 8/8/2016.
//

#include "DebugFontView.h"
#include "loadShaders.h"
#include "errorChecks.h"

namespace DualityEngine {

    bool DebugFontView::init(FontDescriptor *loadedFont, std::stringstream &out, float x, float y, float z) {
        font = loadedFont;

        if (font->sdf) {
            shdrLoc = loadShaders("textureViewSdf.vert", "textureViewSdf.frag", out);
            unifLoc_pxAdvX = glGetUniformLocation(shdrLoc, "pxAdvX");
            unifLoc_pxAdvY = glGetUniformLocation(shdrLoc, "pxAdvY");
            unifLoc_fzzRad = glGetUniformLocation(shdrLoc, "fuzzRadius");
        } else {
            shdrLoc = loadShaders("textureView.vert", "textureView.frag", out);
        }
        attrLoc_verts = glGetAttribLocation(shdrLoc, "Vertex");
        attrLoc_uvCoo = glGetAttribLocation(shdrLoc, "UV");
        unifLoc_txtre = glGetUniformLocation(shdrLoc, "tex");
        unifLoc_mvpMx = glGetUniformLocation(shdrLoc, "MVP");

        glGenVertexArrays(1, &vaoLoc);
        glBindVertexArray(vaoLoc);
        glGenBuffers(2, buffers);

        float halfW = (float)font->getAtlasWidth() / (float)font->getAtlasHeight();
        float halfH = 1.f;
        std::vector<float> verts = {
                x + -halfW, y + -halfH, z + 0.f,
                x +  halfW, y + -halfH, z + 0.f,
                x +  halfW, y +  halfH, z + 0.f,
                x +  halfW, y +  halfH, z + 0.f,
                x + -halfW, y +  halfH, z + 0.f,
                x + -halfW, y + -halfH, z + 0.f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)attrLoc_verts);
        glVertexAttribPointer((GLuint)attrLoc_verts, 3, GL_FLOAT, GL_FALSE, 0, 0);

        std::vector<float> uvs = {
                0.f, 1.f,
                1.f, 1.f,
                1.f, 0.f,
                1.f, 0.f,
                0.f, 0.f,
                0.f, 1.f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)attrLoc_uvCoo);
        glVertexAttribPointer((GLuint)attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0);

        checkError(out, "TextField.cpp", __LINE__);
        return true;
    }

    void DebugFontView::draw(CameraFree *camera) {
        glBindVertexArray(vaoLoc);
        glUseProgram(shdrLoc);
        // TODO: this is wrong: make point to correct tex unit
        glUniform1i(unifLoc_txtre, 1);
        glUniformMatrix4fv(unifLoc_mvpMx, 1, GL_FALSE, &camera->viewProjection[0][0]);
        if (font->sdf) {
            glUniform1f(unifLoc_pxAdvX, 1.f / font->getAtlasWidth());
            glUniform1f(unifLoc_pxAdvY, 1.f / font->getAtlasHeight());
            glUniform1i(unifLoc_fzzRad, 6);
        }
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, font->texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
