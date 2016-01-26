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
#include <sstream>
#include <glm/glm.hpp>
#include <Settings.h>
#include <unordered_map>
#include "Model.h"

namespace DualityEngine {

    struct Mesh {
        std::vector<float> vboCopy;
    };

    struct MeshTableEntry {
        DUA_uint64 vboOffset, vboLength;
        std::vector<DUA_id> instances;
    };

	struct ActiveMeshTableIndex {
		DUA_uint64 entryIndex, instanceIndex;
	};

    class MeshRepository
    {
    private:
        GLuint vboHandle;
        DUA_uint64 vboSize, vboUsed;

        std::vector<Mesh> meshData;
        std::unordered_map<std::string, DUA_uint64> fileToActiveMeshIndex;
        std::unordered_map<DUA_id, ActiveMeshTableIndex> idToActiveMeshIndex;
    public:
		GLuint vaoHandle;
        std::vector<MeshTableEntry> activeMeshes;

        MeshRepository(DUA_uint64 initialVBOsizeInBytes = 8000000);
		bool init(std::stringstream& output);
		void preDrawStateCalls();
        bool registerModel(DUA_id id, Model* model, std::stringstream& output);
        bool deRegisterModel(DUA_id id, std::stringstream& output);
    };

}

#endif	/* MESHREPOSITORY_H */

