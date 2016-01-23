/* 
 * File:   MeshRepository.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 11:57 AM
 */

#ifndef MESHREPOSITORY_H
#define	MESHREPOSITORY_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <Settings.h>
#include <unordered_map>
#include "Model.h"

namespace DualityEngine {

    struct Mesh {
        glm::vec3 vertices;
        glm::vec3 normals;
        glm::vec2 uvs;
    };

    struct MeshTableEntry {
        DUA_uint32 VBOoffset, VBOlength;
        std::vector<DUA_id> instances;
    };

    class MeshRepository
    {
    private:
        GLuint vboHandle;

        std::vector<Mesh> meshData;
        std::unordered_map<std::string, DUA_uint32> tableEntryIndex;
        DUA_uint32 VBOsize, VBOused;
    public:
        std::vector<MeshTableEntry> activeMeshes;
        MeshRepository(DUA_uint32 initialVBOsizeInBytes = 8000000);
        void RegisterModel(Model& model);
    };

}

#endif	/* MESHREPOSITORY_H */

