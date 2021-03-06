//
// Created by adayoldbagel on 1/22/16.
//

#include "MeshRepo.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "errorChecks.h"

namespace DualityEngine {
    MeshRepo::MeshRepo(DUA_uint64 initialVBOsizeInBytes /*= 8000000*/){
        vboSizeBytes = initialVBOsizeInBytes;
        vboSizeVerts = vboSizeBytes / (8 * sizeof(float));
        vboUsedBytes = 0;
        vboUsedVerts = 0;
    }

	bool MeshRepo::init(std::stringstream& output) {
		output << "Beginning initialization of mesh repository...\n";
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		glGenBuffers(1, &vboHandle);
		glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
		glBufferData(GL_ARRAY_BUFFER, vboSizeBytes, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), DUA_GL_BUFFER_OFFSET(0));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), DUA_GL_BUFFER_OFFSET(3 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), DUA_GL_BUFFER_OFFSET(6 * sizeof(float)));

		checkError(output, "MeshRepo.cpp", __LINE__);
		output << "Initialization of mesh repository has finished.\n";
		return true;
	}

    bool MeshRepo::registerModel(DUA_id id, Model* model, std::stringstream& output) {
		try {
			// if there is no matching mesh already loaded - i.e. if no other entity yet uses this model...
			if (!fileToActiveMeshIndex.count(model->meshFileName)) {
				meshData.emplace_back(Mesh());
				Mesh* pNewMesh = &meshData.back();
				// fill newMesh interleaved array...
				{
					// Create an instance of the Importer class
					Assimp::Importer importer;
					// And have it read the given file with some postprocessing
					const aiScene* scene = importer.ReadFile(model->meshFileName, 0);

					// If the import failed, report it
					if (!scene) {
						output << importer.GetErrorString() << std::endl;
						return false;
					}

					// Now file's contents accessible - cement all meshes in file together for now.
                    // This functionality may not be ideal in the future.

                    for (unsigned j = 0; j < scene->mNumMeshes; ++j) {
                        aiMesh *m = scene->mMeshes[j];

                        // Do the interleaving
                        for (unsigned i = 0; i < m->mNumVertices; ++i) {
                            pNewMesh->vboCopy.push_back(m->mVertices[i].x);
                            pNewMesh->vboCopy.push_back(m->mVertices[i].y);
                            pNewMesh->vboCopy.push_back(m->mVertices[i].z);
                            pNewMesh->vboCopy.push_back(m->mNormals[i].x);
                            pNewMesh->vboCopy.push_back(m->mNormals[i].y);
                            pNewMesh->vboCopy.push_back(m->mNormals[i].z);
                            if (m->HasTextureCoords(0)) {
                                pNewMesh->vboCopy.push_back(m->mTextureCoords[0][i].x);
                                pNewMesh->vboCopy.push_back(m->mTextureCoords[0][i].y);
                            } else {
                                pNewMesh->vboCopy.push_back(0.f);
                                pNewMesh->vboCopy.push_back(0.f);
                            }
                        }
                    }
				}

				MeshTableEntry newEntry;
				newEntry.vboOffsetBytes = vboUsedBytes;
                newEntry.vboOffsetVerts = (GLsizei)vboUsedVerts;
				newEntry.vboLengthBytes = pNewMesh->vboCopy.size() * sizeof(float);
                newEntry.vboLengthVerts = (GLsizei)pNewMesh->vboCopy.size() / 8;
				vboUsedBytes += newEntry.vboLengthBytes;
                vboUsedVerts += newEntry.vboLengthVerts;

                if (vboUsedBytes >= vboSizeBytes) {
                    // DO A VBO EXPANSION
                    vboSizeBytes *= 2;
                    vboSizeVerts *= 2;
                    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
                    glBufferData(GL_ARRAY_BUFFER, vboSizeBytes, NULL, GL_DYNAMIC_DRAW);
                    unsigned long currentVboHead = 0;
                    for (unsigned long i = 0; i < meshData.size(); ++i) {
                        glBufferSubData(GL_ARRAY_BUFFER, currentVboHead, meshData.at(i).vboCopy.size() * sizeof(float),
                                        &meshData.at(i).vboCopy[0]);
                    }
                    output << "\nVBO expanded.\n";
                }

				// do VBO upload
				glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
				glBufferSubData(GL_ARRAY_BUFFER, newEntry.vboOffsetBytes, newEntry.vboLengthBytes,
                                &pNewMesh->vboCopy[0]);

				// add to activeMeshes
				activeMeshes.push_back(newEntry);
				// add to fileToActiveMeshIndex
				fileToActiveMeshIndex[model->meshFileName] = activeMeshes.size() - 1;
			}

			DUA_uint64 activeMeshIndex = fileToActiveMeshIndex.at(model->meshFileName);
			DUA_uint64 instanceIndex = activeMeshes.at(activeMeshIndex).instances.size();
			// add id to active mesh instances list
			activeMeshes.at(activeMeshIndex).instances.push_back(id);
			// add to idToActiveMeshIndex
			idToActiveMeshIndex[id] = { activeMeshIndex, instanceIndex };

			return true;
		}
		catch (...) {
			output << "Mesh Repository: exception registering model for id " << id << std::endl;
			return false;
		}
    }

    bool MeshRepo::deRegisterModel(DUA_id id, std::stringstream& output) {
		try {
			std::vector<DUA_id>* instanceList = &activeMeshes.at(idToActiveMeshIndex.at(id).entryIndex).instances;
			DUA_uint64 instanceIndex = idToActiveMeshIndex.at(id).instanceIndex;
			std::swap(instanceList->at(instanceIndex), instanceList->back());
			instanceList->pop_back();
			return true;
		}
		catch (...) {
			output << "Mesh Repository: exception removing model instance for id " << id << std::endl;
			return false;
		}
    }

	void MeshRepo::clean() {
		vboUsedBytes = 0;
		vboUsedVerts = 0;
		meshData.clear();
        activeMeshes.clear();
        fileToActiveMeshIndex.clear();
        idToActiveMeshIndex.clear();
	}
}