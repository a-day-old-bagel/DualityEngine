/* 
 * File:   Model.h
 * Author: adayoldbagel
 *
 * Created on February 9, 2015, 10:42 PM
 */

#ifndef MODEL_H
#define	MODEL_H

#include <string>
#include "Component.h"

namespace DualityEngine {

    struct Model : public Component
    {
        Model(const char* fileName){
            meshFileName = fileName;
            isRegistered = true;
        }
        std::string meshFileName;
        bool isRegistered;
    };

}

#endif	/* MODEL_H */

