/*******************************************************************************
 * File:   Owner.h
 * Author: Galen Cochrane *
 * Created on April 30, 2015, 12:54 PM
 * 
 * Provides the ID of the entity that "owns" the entity possessing this component.
 * Useful for knowing, for example, from what entity came the bullet that killed
 * an enemy, so that the score of the owner entity can be incremented.
 ******************************************************************************/

#ifndef OWNER_H
#define	OWNER_H

#include "Settings.h"

namespace DualityEngine {
    
    struct Owner
    {
        Owner(const DUA_id &ref){
            entityRef = ref;
        }
        DUA_id entityRef;
    };
}

#endif	/* OWNER_H */

