/* 
 * File:   Soul.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef SOUL_H
#define	SOUL_H

#include <string>

namespace DualityEngine {

    struct Soul
    {
        Soul(const char* name, const DUA_COMPFLAG &flags, const DUA_STATEFLAG &states){
            this->name = name; components = flags; state = states;
        }
        std::string name;
        DUA_COMPFLAG components;
        DUA_STATEFLAG state;
    };

}

#endif	/* STATE_H */

