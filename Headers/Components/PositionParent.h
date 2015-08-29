/*******************************************************************************
 * File:   PositionParent.h
 * Author: Galen Cochrane *
 * Created on April 30, 2015, 12:31 PM
 * 
 * Provides the ID of the entity whose position information is inherited by the
 * entity possessing this component.  Useful for position inheritance like that of
 * a weapon to the entity holding it (or to that entity's limb bone or whatever).
 ******************************************************************************/

#ifndef POSITIONPARENT_H
#define	POSITIONPARENT_H

#include "Settings.h"
#include "Component.h"

namespace DualityEngine {
    
    struct SpatialParent : public Component
    {
        SpatialParent(const DUA_id &ref){
            entityRef = ref;
        }
        DUA_id entityRef;
    };
}

#endif	/* POSITIONPARENT_H */

