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

namespace DualityEngine {
    
    struct PositionParent
    {
        PositionParent(const DU_ID &ref){
            entityRef = ref;
        }
        DU_ID entityRef;
    };
}

#endif	/* POSITIONPARENT_H */

