/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

IDNUM ComponentBank::createBox(const char* name,
                                  const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ,
                                  const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ,
                                  const FLOAT &velX, const FLOAT &velY, const FLOAT &velZ,
                                  const FLOAT &anvX, const FLOAT &anvY, const FLOAT &anvZ)
{
    IDNUM newEntityID = createEntity(name);
    if (newEntityID != NULL_ID){
    
        addModel   (newEntityID);
        addMotion  (newEntityID, velX, velY, velZ, anvX, anvY, anvZ);
        addSpatial (newEntityID, posX, posY, posZ, rotX, rotY, rotZ);

        notifySystemsOfAdditions(newEntityID);    
    }
    
    return newEntityID;
}