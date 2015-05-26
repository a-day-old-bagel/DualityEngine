/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

DUA_id ComponentBank::createBox(const char* name,
                               const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ,
                               const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ,   
                               const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ,
                               const DUA_dbl &angX, const DUA_dbl &angY, const DUA_dbl &angZ)
{
    DUA_id newEntityID = createEntity(name);
    if (newEntityID != DUA_NULL_ID){
    
        addModel            (newEntityID);
        addPosition         (newEntityID, posX, posY, posZ);
        addPositionVeloc    (newEntityID, velX, velY, velZ);
        addRotation         (newEntityID, rotX, rotY, rotZ);
        addRotationVeloc    (newEntityID, angX, angY, angZ);
        
    }
    
    return newEntityID;
}