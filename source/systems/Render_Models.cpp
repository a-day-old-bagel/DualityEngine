/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "Render_Models.h"
#include "errorChecks.h"
#include "loaders/loadShaders.h"
#include "extendGlm.h"

namespace DualityEngine {

    System_Render_Models::System_Render_Models(Bank* bank)
            : System<System_Render_Models>(bank, "Model Rendering System", 1) {
        requiredComponents.at(0) = MODEL | POSITION | ORIENTATION;
    }

    bool System_Render_Models::onInit(std::stringstream& engineOut) {
        if (!repo.init(engineOut)) {
            return false;
        }

        shdrLoc = loadShaders("repoDefault.vert", "repoDefault.frag", engineOut);
        unifLoc_MVP = (GLuint) glGetUniformLocation(shdrLoc, "MVP");
        unifLoc_M = (GLuint) glGetUniformLocation(shdrLoc, "M");

        checkError(engineOut, "Render_Models.cpp", __LINE__);

        return true;
    }

    void System_Render_Models::onTick() {
        std::stringstream output;

        for (unsigned long i = 0; i < registeredIDs[0].size(); ++i) {
            Model* model = bank->getModelPtr(registeredIDs[0][i]);
            if (!model->isRegistered) {
                repo.registerModel(registeredIDs[0][i], model, output);
                model->isRegistered = true;
            }
        }

        if (bank->activeFreeCameraID != DUA_NULL_ID) {
            glBindVertexArray(repo.vaoHandle);
            DUA_uint32 time = bank->getTime();
            bank->updateActiveCamera(time); // Camera now updated here for all rendering systems
            glm::mat4 vp = bank->pFreeCameraCurrent->viewProjection;
            for (unsigned long i = 0; i < repo.activeMeshes.size(); ++i) {
                MeshTableEntry* mesh = &repo.activeMeshes.at(i);
                for (unsigned long j = 0; j < mesh->instances.size(); ++j) {
                    DUA_id id = mesh->instances.at(j);
                    glm::mat4 m = bank->getModMat(id, time);
                    glm::mat4 mvp = vp * m;
                    glUseProgram(shdrLoc);
                    glUniformMatrix4fv(unifLoc_MVP, 1, GL_FALSE, &mvp[0][0]);
                    glUniformMatrix4fv(unifLoc_M, 1, GL_FALSE, &m[0][0]);
                    glDrawArrays(GL_TRIANGLES, mesh->vboOffsetVerts, mesh->vboLengthVerts);
                }
            }
        }

        if (output.rdbuf()->in_avail() > 0) {
            bank->dlgt->outputStr(output.str());
        }
    }


    void System_Render_Models::onClean() {

    }

    void System_Render_Models::forceRemoveComp(const DUA_id& ID, const DUA_compFlag& component) {
        if (System<System_Render_Models>::forceRemoveComp(ID, component)) {
            std::stringstream output;
            repo.deRegisterModel(ID, output);
            bank->dlgt->outputStr(output.str());
        }
    }
}
