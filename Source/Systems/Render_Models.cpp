/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "Render_Models.h"
#include "Render_Master.h"
#include "loadShaders.h"

using namespace DualityEngine;

System_Render_Models::System_Render_Models(ComponentBank* bank)
                  : System<System_Render_Models>(bank, "Model Rendering System", 1)
{
    requiredComponents.at(0) = MODEL | POSITION | ORIENTATION;
}

System_Render_Models::~System_Render_Models()
{
    
}

bool System_Render_Models::init(std::stringstream& engineOut)
{
    if (!repo.init(engineOut)) {
        return false;
    }

    shdrLoc = loadShaders("Assets/Shaders/repoDefault.vert", "Assets/Shaders/repoDefault.frag", engineOut);
    unifLoc_MVP = (GLuint)glGetUniformLocation(shdrLoc, "MVP");
    unifLoc_M = (GLuint)glGetUniformLocation(shdrLoc, "M");

	System_Render_Master::checkError(engineOut, "Render_Models.cpp", __LINE__);

    return true;
}

void vec3Interpolate(glm::vec3& lhs, glm::vec3& rhs, glm::vec3& result, float interp) {
    glm::vec3 diff = rhs - lhs;
    result = lhs + interp * diff;
}

void System_Render_Models::tick()
{
    std::stringstream output;

    for (long unsigned i = 0; i < registeredIDs[0].size(); ++i) {
        Model* model = bank->getModelPtr(registeredIDs[0][i]);
        if (!model->isRegistered) {
            repo.registerModel(registeredIDs[0][i], model, output);
            model->isRegistered = true;
        }
    }

    if (bank->activeFreeCameraID != DUA_NULL_ID) {
        glBindVertexArray(repo.vaoHandle);
        glm::mat4 vp = bank->pFreeCameraCurrent->viewProjection;
        DUA_uint32 time = SDL_GetTicks();
        for (unsigned long i = 0; i < repo.activeMeshes.size(); ++i) {
            MeshTableEntry* mesh = &repo.activeMeshes.at(i);
            for (unsigned long j = 0; j < mesh->instances.size(); ++j) {
                DUA_id id = mesh->instances.at(j);
                glm::mat4 m = bank->getModMat(id);

                if (bank->getComponents(id) & DUA_componentFlags::LINVELOC) {
                    Position *pos = bank->getPositionPtr(id);
                    glm::vec3 interpolatedPosition;
                    vec3Interpolate(pos->lastPosition, pos->position, interpolatedPosition,
                                    (float) (time - pos->lastTimeStamp) /
                                    (float) (pos->timeStamp - pos->lastTimeStamp));
                    m[3][0] = interpolatedPosition.x;
                    m[3][1] = interpolatedPosition.y;
                    m[3][2] = interpolatedPosition.z;
                }

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

void System_Render_Models::forceRemoveComp(const DUA_id& ID, const DUA_compFlag& component) {
    if (System<System_Render_Models>::forceRemoveComp(ID, component)) {
        std::stringstream output;
        repo.deRegisterModel(ID, output);
        bank->dlgt->outputStr(output.str());
    }
}