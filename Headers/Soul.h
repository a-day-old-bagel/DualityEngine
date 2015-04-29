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
        std::string name;
        componentFlag components;
        stateFlag state;
    };

}

#endif	/* STATE_H */

