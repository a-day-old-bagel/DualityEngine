/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "errorChecks.h"
#include "Render_BiTcubes.h"

namespace DualityEngine {

#define vUN  0.0 ,  0.5 , -0.25
#define vUS  0.0 ,  0.5 ,  0.25
#define vUE  0.25,  0.5 ,  0.0
#define vUW -0.25,  0.5 ,  0.0
#define vDN  0.0 , -0.5 , -0.25
#define vDS  0.0 , -0.5 ,  0.25
#define vDE  0.25, -0.5 ,  0.0
#define vDW -0.25, -0.5 ,  0.0
#define vNU  0.0 ,  0.25, -0.5
#define vND  0.0 , -0.25, -0.5
#define vNE  0.25,  0.0 , -0.5
#define vNW -0.25,  0.0 , -0.5
#define vSU  0.0 ,  0.25,  0.5
#define vSD  0.0 , -0.25,  0.5
#define vSE  0.25,  0.0 ,  0.5
#define vSW -0.25,  0.0 ,  0.5
#define vEU  0.5 ,  0.25,  0.0
#define vED  0.5 , -0.25,  0.0
#define vEN  0.5 ,  0.0 , -0.25
#define vES  0.5 ,  0.0 ,  0.25
#define vWU -0.5 ,  0.25,  0.0
#define vWD -0.5 , -0.25,  0.0
#define vWN -0.5 ,  0.0 , -0.25
#define vWS -0.5 ,  0.0 ,  0.25

#define nU__    0.0  ,  1.0  ,  0.0
#define nD__    0.0  , -1.0  ,  0.0
#define nN__    0.0  ,  0.0  , -1.0
#define nS__    0.0  ,  0.0  ,  1.0
#define nE__    1.0  ,  0.0  ,  0.0
#define nW__   -1.0  ,  0.0  ,  0.0
#define nUNE    0.577350269  ,  0.577350269  , -0.577350269
#define nUNW   -0.577350269  ,  0.577350269  , -0.577350269
#define nUSE    0.577350269  ,  0.577350269  ,  0.577350269
#define nUSW   -0.577350269  ,  0.577350269  ,  0.577350269
#define nDNE    0.577350269  , -0.577350269  , -0.577350269
#define nDNW   -0.577350269  , -0.577350269  , -0.577350269
#define nDSE    0.577350269  , -0.577350269  ,  0.577350269
#define nDSW   -0.577350269  , -0.577350269  ,  0.577350269


    System_Render_BiTcubes::System_Render_BiTcubes(Bank* bank)
            : System<System_Render_BiTcubes>(bank, "BiTCube Rendering System", 0) { }

    bool System_Render_BiTcubes::onInit(std::stringstream& engineOut) {
        checkError(engineOut, "Render_BiTCubes.cpp", __LINE__);
        return true;
    }

    void System_Render_BiTcubes::onTick() {

    }

    void System_Render_BiTcubes::onClean() {

    }
}
