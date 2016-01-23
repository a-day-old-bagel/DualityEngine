//
// Created by adayoldbagel on 1/22/16.
//

#include "MeshRepository.h"

namespace DualityEngine {
    MeshRepository::MeshRepository(DUA_uint32 initialVBOsizeInBytes /*= 8000000*/){
        VBOsize = initialVBOsizeInBytes;
        VBOused = 0;

        glGenBuffers(1, &vboHandle);

        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glBufferData(GL_ARRAY_BUFFER, VBOsize, NULL, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), DUA_GL_BUFFER_OFFSET(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), DUA_GL_BUFFER_OFFSET(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), DUA_GL_BUFFER_OFFSET(6 * sizeof(float)));
    }

    void MeshRepository::RegisterModel(Model &model) {
        if (!tableEntryIndex.count(model.meshFileName)) {
            Mesh newMesh;
            newMesh.vertices = {};
            newMesh.normals = {};
            newMesh.uvs = {};
            meshData.push_back(newMesh);
            //tableEntryIndex.
        }
        MeshTableEntry* entry = &(activeMeshes.at(tableEntryIndex.at(model.meshFileName)));
    }
}