/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "Bank.h"

using namespace DualityEngine;

DUA_id Bank::createBox(const char *name,
                       const DUA_dbl posX, const DUA_dbl posY, const DUA_dbl posZ,
                       const DUA_dbl rotX, const DUA_dbl rotY, const DUA_dbl rotZ,
                       const DUA_dbl velX, const DUA_dbl velY, const DUA_dbl velZ,
                       const DUA_dbl angX, const DUA_dbl angY, const DUA_dbl angZ)
{
    DUA_id newEntityID = spawnEntity(name);
    if (newEntityID != DUA_NULL_ID){
    
        addModel         (newEntityID, "assets/models/SimpleCube/cube.obj");
        addPosition      (newEntityID, posX, posY, posZ);
        addLinearVeloc   (newEntityID, velX, velY, velZ);
        addOrientation   (newEntityID, rotX, rotY, rotZ);
        addAngularVeloc  (newEntityID, angX, angY, angZ);
        
    }
    
    return newEntityID;
}