#ifndef DEBUGCUBE_H
#define DEBUGCUBE_H

#include "../../../Headers/Settings.h"

namespace DualityEngine {

   /****************************************************************************
    * DEBUG CUBE
    * This is a hard-coded 3D model of a cube I wrote in order to debug asset
    * loading and the rendering system in general. It was also a useful
    * exercise in remembering the general format of 3D object data.
    * This could have been optimized with a better UV unfolding of the cube
    * (this is the worst case, where all vertices are unwelded),
    * but I didn't want to try unfolding to literal values in my head.
    ***************************************************************************/
    struct DebugCube{
        // Vertices of the cube mesh
        DU_VEC3 vertices[24] = {    // A cube has 8 corners, each of which are represented three times - once for each connecting face.
                                    // Redundant vertices are nearly unavoidable when UV mapping 3D objects, but usually this is hidden from the developer.
                                    // Vertex count increases from 8 to 24, but the number of triangles that must be rendered does not.
            { 1.0,  1.0,  1.0},
            { 1.0,  1.0,  1.0},     // Corner 1
            { 1.0,  1.0,  1.0},

            { 1.0,  1.0, -1.0},
            { 1.0,  1.0, -1.0},     // Corner 2
            { 1.0,  1.0, -1.0},

            { 1.0, -1.0,  1.0},
            { 1.0, -1.0,  1.0},     // Corner 3
            { 1.0, -1.0,  1.0},

            { 1.0, -1.0, -1.0},
            { 1.0, -1.0, -1.0},     // Corner 4
            { 1.0, -1.0, -1.0},

            {-1.0,  1.0,  1.0},
            {-1.0,  1.0,  1.0},     // Corner 5
            {-1.0,  1.0,  1.0},

            {-1.0,  1.0, -1.0},
            {-1.0,  1.0, -1.0},     // Corner 6
            {-1.0,  1.0, -1.0},

            {-1.0, -1.0,  1.0},
            {-1.0, -1.0,  1.0},     // Corner 7
            {-1.0, -1.0,  1.0},

            {-1.0, -1.0, -1.0},
            {-1.0, -1.0, -1.0},     // Corner 8
            {-1.0, -1.0, -1.0}
        };
        // Normals for each vertex
        DU_VEC3 normals[24] = {     // Each of the three corner vertices has a respective normal pointing in the direction of the face the vertex belongs to.
                                    // This will create a 'flat shaded' look to the cube, which is to say the edges will look 'sharp'.
            { 0.0,  0.0,  1.0},
            { 0.0,  1.0,  0.0},           // Corner 1 connects 3 faces - one pointing in the z direction, one in the y, and one in the x.
            { 1.0,  0.0,  0.0},
            
            { 0.0,  0.0, -1.0},
            { 0.0,  1.0,  0.0},           // Corner 2
            { 1.0,  0.0,  0.0},
            
            { 0.0,  0.0,  1.0},
            { 0.0, -1.0,  0.0},           // Corner 3
            { 1.0,  0.0,  0.0},
            
            { 0.0,  0.0, -1.0},
            { 0.0, -1.0,  0.0},           // Corner 4
            { 1.0,  0.0,  0.0},
            
            { 0.0,  0.0,  1.0},
            { 0.0,  1.0,  0.0},           // Corner 5
            {-1.0,  0.0,  0.0},
            
            { 0.0,  0.0, -1.0},
            { 0.0,  1.0,  0.0},           // Corner 6
            {-1.0,  0.0,  0.0},
            
            { 0.0,  0.0,  1.0},
            { 0.0, -1.0,  0.0},           // Corner 7
            {-1.0,  0.0,  0.0},
            
            { 0.0,  0.0, -1.0},
            { 0.0, -1.0,  0.0},           // Corner 8
            {-1.0,  0.0,  0.0}
        };
        // convenient define of one third for the following...
        const DU_FLOAT thrd = 1.0 / 3.0;
        // This is just a helper to create the correct UVs. It will not be used as object data.
        // the values here are based on the layout of the texture image.
        DU_VEC2 faces[6][4] = {
            {{0.0,    0.0}, {0.5,    0.0}, {0.0,   thrd}, {0.5,   thrd}},
            {{0.5,    0.0}, {1.0,    0.0}, {0.5,   thrd}, {1.0,   thrd}},
            {{0.0,   thrd}, {0.5,   thrd}, {0.0, 2*thrd}, {0.5, 2*thrd}},
            {{0.5,   thrd}, {1.0,   thrd}, {0.5, 2*thrd}, {1.0, 2*thrd}},
            {{0.0, 2*thrd}, {0.5, 2*thrd}, {0.0,    1.0}, {0.5,    1.0}},
            {{0.5, 2*thrd}, {1.0, 2*thrd}, {0.5,    1.0}, {1.0,    1.0}}            
        };
        // UV texture coordinates for each vertex (ranging from 0.0 to 1.0 for each x and y)
        DU_VEC2 uvCoords[24] = {
            faces[1][0],
            faces[4][1],            // Corner 1
            faces[3][1],
            
            faces[0][1],
            faces[4][3],            // Corner 2
            faces[3][0],
            
            faces[1][2],
            faces[5][3],            // Corner 3
            faces[3][3],
            
            faces[0][3],
            faces[5][1],            // Corner 4
            faces[3][2],
            
            faces[1][1],
            faces[4][0],            // Corner 5
            faces[2][0],
            
            faces[0][0],
            faces[4][2],            // Corner 6
            faces[2][1],
            
            faces[1][3],
            faces[5][2],            // Corner 7
            faces[2][2],
            
            faces[0][2],
            faces[5][0],            // Corner 8
            faces[2][3]
        };
        // these define the colored pixels that will be used in the bitmap data below.
        #define EDGE 0xFF, 0xCD, 0x77
        #define CRNR 0xFF, 0xC9, 0x61
        #define BGND 0xC7, 0xC7, 0xC7
        #define CLR1 0xFF, 0xA2, 0x28
        #define CLR2 0x9A, 0xE6, 0x00
        #define CLR3 0xEE, 0x49, 0xFF
        #define CLR4 0x00, 0x10, 0xEC
        #define CLR5 0xEC, 0xEC, 0x00
        #define CLR6 0xFF, 0x33, 0x33
        // raw bitmap data for the texture:
        DU_COLORBYTE rawTexture[1152] = { // 16 x 24 x 3 (width x height x [RGB]) = 1152 bytes
            CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR, CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR,
            EDGE, BGND, BGND, CLR1, CLR1, BGND, BGND, EDGE, EDGE, BGND, BGND, CLR2, CLR2, BGND, BGND, EDGE,
            EDGE, BGND, BGND, BGND, CLR1, BGND, BGND, EDGE, EDGE, BGND, CLR2, BGND, BGND, CLR2, BGND, EDGE, 
            EDGE, BGND, BGND, BGND, CLR1, BGND, BGND, EDGE, EDGE, BGND, BGND, BGND, CLR2, BGND, BGND, EDGE, 
            EDGE, BGND, BGND, BGND, CLR1, BGND, BGND, EDGE, EDGE, BGND, BGND, CLR2, BGND, BGND, BGND, EDGE, 
            EDGE, BGND, BGND, BGND, CLR1, BGND, BGND, EDGE, EDGE, BGND, CLR2, BGND, BGND, BGND, BGND, EDGE, 
            EDGE, BGND, BGND, CLR1, CLR1, CLR1, BGND, EDGE, EDGE, BGND, CLR2, CLR2, CLR2, CLR2, BGND, EDGE, 
            CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR, CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR,
            CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR, CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR,
            EDGE, BGND, CLR3, CLR3, CLR3, BGND, BGND, EDGE, EDGE, BGND, CLR4, BGND, CLR4, BGND, BGND, EDGE, 
            EDGE, BGND, BGND, BGND, BGND, CLR3, BGND, EDGE, EDGE, BGND, CLR4, BGND, CLR4, BGND, BGND, EDGE, 
            EDGE, BGND, CLR3, CLR3, CLR3, BGND, BGND, EDGE, EDGE, BGND, CLR4, BGND, CLR4, BGND, BGND, EDGE, 
            EDGE, BGND, BGND, BGND, BGND, CLR3, BGND, EDGE, EDGE, BGND, CLR4, CLR4, CLR4, CLR4, BGND, EDGE, 
            EDGE, BGND, BGND, BGND, BGND, CLR3, BGND, EDGE, EDGE, BGND, BGND, BGND, CLR4, BGND, BGND, EDGE, 
            EDGE, BGND, CLR3, CLR3, CLR3, BGND, BGND, EDGE, EDGE, BGND, BGND, BGND, CLR4, BGND, BGND, EDGE, 
            CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR, CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR,
            CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR, CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR,
            EDGE, BGND, CLR5, CLR5, CLR5, CLR5, BGND, EDGE, EDGE, BGND, BGND, CLR6, CLR6, CLR6, BGND, EDGE, 
            EDGE, BGND, CLR5, BGND, BGND, BGND, BGND, EDGE, EDGE, BGND, CLR6, BGND, BGND, BGND, BGND, EDGE, 
            EDGE, BGND, CLR5, CLR5, CLR5, BGND, BGND, EDGE, EDGE, BGND, CLR6, BGND, BGND, BGND, BGND, EDGE, 
            EDGE, BGND, BGND, BGND, BGND, CLR5, BGND, EDGE, EDGE, BGND, CLR6, CLR6, CLR6, BGND, BGND, EDGE, 
            EDGE, BGND, BGND, BGND, BGND, CLR5, BGND, EDGE, EDGE, BGND, CLR6, BGND, BGND, CLR6, BGND, EDGE,
            EDGE, BGND, CLR5, CLR5, CLR5, BGND, BGND, EDGE, EDGE, BGND, BGND, CLR6, CLR6, BGND, BGND, EDGE, 
            CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR, CRNR, EDGE, EDGE, EDGE, EDGE, EDGE, EDGE, CRNR
        };
    };
}

#endif
