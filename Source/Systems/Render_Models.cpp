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
    // Load graphics assets and buffer them to GPU
    if(!setUpResources(engineOut)) {
        engineOut << "<!>    Unable to initialize graphics resources!" << std::endl;
        return false;
    }
	System_Render_Master::checkError(engineOut, "Render_Models.cpp", __LINE__);
    return true;
}

bool System_Render_Models::setUpResources(std::stringstream& engineOut)
{
    bool success = true;
    
    success &= debugCube.Init(engineOut);
	success &= repo.init(engineOut);

	//testBoxId = bank->createBox("testBox", 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.);
	//success &= repo.registerModel(testBoxId, bank->getModelPtr(testBoxId), engineOut);

	/*shdrLoc = loadShaders("Assets/Shaders/simpleCube.vert", "Assets/Shaders/simpleCube.frag", engineOut);
	attrLoc_verts = glGetAttribLocation(shdrLoc, "Vertex");
	attrLoc_norms = glGetAttribLocation(shdrLoc, "Normal");
	attrLoc_uvCoo = glGetAttribLocation(shdrLoc, "UV");
	unifLoc_MVP = glGetUniformLocation(shdrLoc, "MVP");
	unifLoc_M = glGetUniformLocation(shdrLoc, "M");*/
             
    return success;
}

void System_Render_Models::tick()
{
	
	/*if (bank->activeFreeCameraID != DUA_NULL_ID) {
		glBindVertexArray(repo.vaoHandle);
		glm::mat4 m = bank->getModMat(testBoxId);
		glm::mat4 mvp = bank->pFreeCameraCurrent->viewProjection * m;
		glUseProgram(shdrLoc);
		glUniformMatrix4fv(unifLoc_MVP, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(unifLoc_M, 1, GL_FALSE, &m[0][0]);
		glDrawArrays(GL_TRIANGLES, repo.activeMeshes.at(0).vboOffset, repo.activeMeshes.at(0).vboLength);
	}	*/

    if (bank->activeFreeCameraID != DUA_NULL_ID){
		for (unsigned i = 0; i < registeredIDs[0].size(); ++i) {
			debugCube.render(bank->getModMat(registeredIDs[0][i]), bank->pFreeCameraCurrent->viewProjection);
		}
    }
}