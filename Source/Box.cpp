/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

DUA_ID ComponentBank::createBox(const char* name,
                               const DUA_DBL &posX, const DUA_DBL &posY, const DUA_DBL &posZ,
                               const DUA_DBL &rotX, const DUA_DBL &rotY, const DUA_DBL &rotZ,   
                               const DUA_DBL &velX, const DUA_DBL &velY, const DUA_DBL &velZ,
                               const DUA_DBL &angX, const DUA_DBL &angY, const DUA_DBL &angZ)
{
    DUA_ID newEntityID = createEntity(name);
    if (newEntityID != DUA_NULL_ID){
    
        addModel            (newEntityID);
        addPosition         (newEntityID, posX, posY, posZ);
        addPositionVeloc    (newEntityID, velX, velY, velZ);
        addRotation         (newEntityID, rotX, rotY, rotZ);
        addRotationVeloc    (newEntityID, angX, angY, angZ);
        
    }
    
    return newEntityID;
}