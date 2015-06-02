/* 
 * File:   Model.h
 * Author: adayoldbagel
 *
 * Created on February 9, 2015, 10:42 PM
 */

#ifndef MODEL_H
#define	MODEL_H

#include <vector>
#include "Component.h"

namespace DualityEngine {

    struct Model : public Component
    {
        Model(const char* fileName){
            
        }
        //Mesh* mesh;
        /*
         * GLuint* shaders;
         * Type* textures;
         * GLuint etc
         */
    };

}

#endif	/* MODEL_H */

