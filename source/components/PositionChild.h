/*******************************************************************************
 * File:   SpatialChild.h
 * Author: Galen Cochrane *
 * Created on April 30, 2015, 3:19 PM
 * 
 * 
 ******************************************************************************/

#ifndef POSITIONCHILD_H
#define	POSITIONCHILD_H

#include "settings.h"
#include "Component.h"

namespace DualityEngine {
    
    struct SpatialChild : public Component
    {
        SpatialChild(const DUA_id &ref){
            entityRef = ref;
        }
        DUA_id entityRef;
    };
}

#endif	/* POSTIONCHILD_H */

