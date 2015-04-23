/* 
 * File:   Soul.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef SOUL_H
#define	SOUL_H

#include <string>
#include "Component.h"

struct Soul : Component
{
    std::string name            = "nameless";
    componentFlag components    = 0;
    stateFlag state             = 0;
};

#endif	/* STATE_H */

