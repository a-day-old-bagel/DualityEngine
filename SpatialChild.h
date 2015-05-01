/*******************************************************************************
 * File:   SpatialChild.h
 * Author: Galen Cochrane *
 * Created on April 30, 2015, 3:19 PM
 * 
 * 
 ******************************************************************************/

#ifndef SPATIALCHILD_H
#define	SPATIALCHILD_H

#include "Settings.h"

namespace DualityEngine {
    
    struct SpatialChild
    {
        SpatialChild(const IDNUM &ref){
            entityRef = ref;
        }
        IDNUM entityRef;
    };
}

#endif	/* SPATIALCHILD_H */

