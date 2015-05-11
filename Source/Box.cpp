/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

DU_ID ComponentBank::createBox(const char* name,
                               const DU_FLOAT &posX, const DU_FLOAT &posY, const DU_FLOAT &posZ,
                               const DU_FLOAT &rotX, const DU_FLOAT &rotY, const DU_FLOAT &rotZ,   
                               const DU_FLOAT &velX, const DU_FLOAT &velY, const DU_FLOAT &velZ,
                               const DU_FLOAT &angX, const DU_FLOAT &angY, const DU_FLOAT &angZ)
{
    DU_ID newEntityID = createEntity(name);
    if (newEntityID != DU_NULL_ID){
    
        addModel            (newEntityID);
        addPosition         (newEntityID, posX, posY, posZ);
        addPositionVeloc    (newEntityID, velX, velY, velZ);
        addRotation         (newEntityID, rotX, rotY, rotZ);
        addRotationVeloc    (newEntityID, angX, angY, angZ);

        notifySystemsOfAdditions(newEntityID);    
    }
    
    return newEntityID;
}