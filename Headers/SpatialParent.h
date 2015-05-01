/*******************************************************************************
 * File:   SpatialAnchor.h
 * Author: Galen Cochrane *
 * Created on April 30, 2015, 12:31 PM
 * 
 * Provides the ID of the entity whose spatial information is inherited by the
 * entity possessing this component.  Useful for spatial inheritance like that of
 * a weapon to the entity holding it (or to that entity's limb bone or whatever).
 ******************************************************************************/

#ifndef SPATIALANCHOR_H
#define	SPATIALANCHOR_H

#include "Settings.h"

namespace DualityEngine {
    
    struct SpatialParent
    {
        SpatialParent(const IDNUM &ref){
            entityRef = ref;
        }
        IDNUM entityRef;
    };
}

#endif	/* SPATIALANCHOR_H */

