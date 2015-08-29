/* 
 * File:   Component.h
 * Author: adayoldbagel
 */


/*         NOT USED              NOT USED           NOT USED                  NOT USED                !!!!           */


#ifndef COMPONENT_H
#define	COMPONENT_H

namespace DualityEngine {

    struct Component
    {
        template<typename ... types>
        Component(const types& ... args) {}
    };
}

#endif	/* COMPONENT_H */

