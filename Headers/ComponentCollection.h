/* 
 * File:   ComponentCollection.h
 * Author: adayoldbagel
 */


/*         NOT USED              NOT USED           NOT USED                  NOT USED                !!!!           */


#ifndef COMPONENTCOLLECTION_H
#define	COMPONENTCOLLECTION_H

#include "Settings.h"

namespace DualityEngine {

    struct ComponentCollection
    {
        virtual void emplace(const DUA_id& ID) = 0;
        virtual void remove(const DUA_id& ID) = 0;
        template<class componentType>
        virtual componentType* getPtr(const DUA_id& ID) = 0;
    };

}

#endif	/* COMPONENTCOLLECTION_H */

