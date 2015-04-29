/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

IDnumber ComponentBank::createBox(double posX, double posY, double posZ,
                                  double rotX, double rotY, double rotZ,
                                  double velX, double velY, double velZ,
                                  double anvX, double anvY, double anvZ)
{
    IDnumber ID = generateID();
    componentFlag components = MODEL|MOTION|SPATIAL;
    
    addModel   (ID);
    addMotion  (ID, velX, velY, velZ, anvX, anvY, anvZ);
    addSpatial (ID, posX, posY, posZ, rotX, rotY, rotZ);
    addSoul    (ID, "box", components);
    
    return ID;
}