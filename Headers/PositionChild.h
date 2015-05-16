/*******************************************************************************
 * File:   SpatialChild.h
 * Author: Galen Cochrane *
 * Created on April 30, 2015, 3:19 PM
 * 
 * 
 ******************************************************************************/

#ifndef POSITIONCHILD_H
#define	POSITIONCHILD_H

#include "Settings.h"

namespace DualityEngine {
    
    struct PositionChild
    {
        PositionChild(const DUA_ID &ref){
            entityRef = ref;
        }
        DUA_ID entityRef;
    };
}

#endif	/* POSTIONCHILD_H */

