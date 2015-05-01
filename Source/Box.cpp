/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

IDnumber ComponentBank::createBox(const char* name,
                                  double posX, double posY, double posZ,
                                  double rotX, double rotY, double rotZ,
                                  double velX, double velY, double velZ,
                                  double anvX, double anvY, double anvZ)
{
    IDnumber newEntityID = createEntity(name);
    if (newEntityID != NULL_ID){
    
        addModel   (newEntityID);
        addMotion  (newEntityID, velX, velY, velZ, anvX, anvY, anvZ);
        addSpatial (newEntityID, posX, posY, posZ, rotX, rotY, rotZ);

        notifySystems(newEntityID);    
    }
    
    return newEntityID;
}