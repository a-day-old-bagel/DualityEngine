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

namespace DualityEngine {

    struct Soul : public Component
    {
        Soul(const char* name, const DUA_compFlag &flags, const DUA_stateFlag &states){
            this->name = name; components = flags; state = states;
        }
        std::string name;
        DUA_compFlag components;
        DUA_stateFlag state;
    };

}

#endif	/* STATE_H */

