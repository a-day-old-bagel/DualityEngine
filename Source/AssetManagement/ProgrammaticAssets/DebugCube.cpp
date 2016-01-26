//
// Created by adayoldbagel on 1/8/16.
//

#include <sstream>
#include "Settings.h"
#include "loadShaders.h"
#include "DebugCube.h"

// helpers used in the indices below to reference the correct vertex...
#define FACING_X * 3 + 2
#define FACING_Y * 3 + 1
#define FACING_Z * 3
// these define the colored pixels that will be used in the bitmap data below.
#define EDG 0x00, 0xFF, 0xFF // cyan
#define CNR 0x00, 0xB0, 0xB0 // dark cyan
#define BGD 0x3F, 0x3E, 0x30 // light grey
#define CL1 0xFF, 0xA2, 0x28 // orange
#define CL2 0x33, 0xFF, 0x00 // bright green
#define CL3 0xEE, 0x49, 0xFF // barney purple
#define CL4 0x20, 0x20, 0xFF // screen o' death blue
#define CL5 0xFF, 0xFF, 0x00 // yellow
#define CL6 0xFF, 0x33, 0x33 // scarlet

namespace DualityEngine {
    const DUA_float DebugCube::vertices[72] = {
            // A cube has 8 corners, each of which are represented three times - once for each connecting face.
            // Redundant vertices are nearly unavoidable when UV mapping 3D objects, but usually this is hidden from the developer.
            // Vertex count increases from 8 to 24, but the number of triangles that must be rendered does not.
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,     // Corner 0
            0.5f,  0.5f,  0.5f,

            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,     // Corner 1
            0.5f,  0.5f, -0.5f,

            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,     // Corner 2
            0.5f, -0.5f,  0.5f,

            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,     // Corner 3
            0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,     // Corner 4
            -0.5f,  0.5f,  0.5f,

            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,     // Corner 5
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,     // Corner 6
            -0.5f, -0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,     // Corner 7
            -0.5f, -0.5f, -0.5f
    };
    // Normals for each vertex
    const DUA_float DebugCube::normals[72] = {
            // Each of the three corner vertices has a respective normal pointing in the direction of the face the vertex belongs to.
            // This will create a 'flat shaded' look to the cube, which is to say the edges will look 'sharp'.
            0.0,  0.0,  1.f,
            0.f,  1.f,  0.f,         // Corner 0 connects 3 faces - one pointing in the z direction, one in the y, and one in the x.
            1.f,  0.f,  0.f,

            0.f,  0.f, -1.f,
            0.f,  1.f,  0.f,         // Corner 1
            1.f,  0.f,  0.f,

            0.f,  0.f,  1.f,
            0.f, -1.f,  0.f,         // Corner 2
            1.f,  0.f,  0.f,

            0.f,  0.f, -1.f,
            0.f, -1.f,  0.f,         // Corner 3
            1.f,  0.f,  0.f,

            0.f,  0.f,  1.f,
            0.f,  1.f,  0.f,         // Corner 4
            -1.f,  0.f,  0.f,

            0.f,  0.f, -1.f,
            0.f,  1.f,  0.f,         // Corner 5
            -1.f,  0.f,  0.f,

            0.f,  0.f,  1.f,
            0.f, -1.f,  0.f,         // Corner 6
            -1.f,  0.f,  0.f,

            0.f,  0.f, -1.f,
            0.f, -1.f,  0.f,         // Corner 7
            -1.f,  0.f,  0.f
    };
    // This is just a helper to create the correct UVs. It will not be used as object data.
    // the values here are based on the layout of the texture image.
    const glm::vec2 DebugCube::faces[6][4] = {
            {{0.0, 0.75}, {0.5, 0.75}, {0.0,  0.5}, {0.5,  0.5}},
            {{0.5, 0.75}, {1.0, 0.75}, {0.5,  0.5}, {1.0,  0.5}},
            {{0.0,  0.5}, {0.5,  0.5}, {0.0, 0.25}, {0.5, 0.25}},
            {{0.5,  0.5}, {1.0,  0.5}, {0.5, 0.25}, {1.0, 0.25}},
            {{0.0, 0.25}, {0.5, 0.25}, {0.0, 0.00}, {0.5, 0.00}},
            {{0.5, 0.25}, {1.0, 0.25}, {0.5, 0.00}, {1.0, 0.00}}
    };
    // UV texture coordinates for each vertex (ranging from 0.0 to 1.0 for each x and y)
    const DUA_float DebugCube::uvCoords[48] = {
            faces[1][0].x, faces[1][0].y,
            faces[4][1].x, faces[4][1].y,                    // Corner 0
            faces[3][1].x, faces[3][1].y,

            faces[0][1].x, faces[0][1].y,
            faces[4][3].x, faces[4][3].y,                    // Corner 1
            faces[3][0].x, faces[3][0].y,

            faces[1][2].x, faces[1][2].y,
            faces[5][3].x, faces[5][3].y,                    // Corner 2
            faces[3][3].x, faces[3][3].y,

            faces[0][3].x, faces[0][3].y,
            faces[5][1].x, faces[5][1].y,                    // Corner 3
            faces[3][2].x, faces[3][2].y,

            faces[1][1].x, faces[1][1].y,
            faces[4][0].x, faces[4][0].y,                    // Corner 4
            faces[2][0].x, faces[2][0].y,

            faces[0][0].x, faces[0][0].y,
            faces[4][2].x, faces[4][2].y,                    // Corner 5
            faces[2][1].x, faces[2][1].y,

            faces[1][3].x, faces[1][3].y,
            faces[5][2].x, faces[5][2].y,                    // Corner 6
            faces[2][2].x, faces[2][2].y,

            faces[0][2].x, faces[0][2].y,
            faces[5][0].x, faces[5][0].y,                    // Corner 7
            faces[2][3].x, faces[2][3].y
    };

    // face indices specifying faces to be drawn by referencing the vertices
    const DUA_uint8 DebugCube::indices[36] = { // 12 triangles make 6 squares make 1 cube
            5 FACING_Z, 3 FACING_Z, 7 FACING_Z,   // Face 0
            5 FACING_Z, 1 FACING_Z, 3 FACING_Z,   // Face 1...
            0 FACING_Z, 6 FACING_Z, 2 FACING_Z,
            0 FACING_Z, 4 FACING_Z, 6 FACING_Z,
            4 FACING_X, 7 FACING_X, 6 FACING_X,
            4 FACING_X, 5 FACING_X, 7 FACING_X,
            1 FACING_X, 2 FACING_X, 3 FACING_X,
            1 FACING_X, 0 FACING_X, 2 FACING_X,
            4 FACING_Y, 1 FACING_Y, 5 FACING_Y,
            4 FACING_Y, 0 FACING_Y, 1 FACING_Y,
            7 FACING_Y, 2 FACING_Y, 6 FACING_Y,
            7 FACING_Y, 3 FACING_Y, 2 FACING_Y
    };
    // raw bitmap data for the texture:
    const DUA_colorByte DebugCube::rawTexture[1536] = { // 16 x 32 x 3 (width x height x [RGB]) = 1152 bytes
            CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,
            EDG,BGD,BGD,CL1,CL1,BGD,BGD,EDG,EDG,BGD,BGD,CL2,CL2,BGD,BGD,EDG,
            EDG,BGD,BGD,BGD,CL1,BGD,BGD,EDG,EDG,BGD,CL2,BGD,BGD,CL2,BGD,EDG,
            EDG,BGD,BGD,BGD,CL1,BGD,BGD,EDG,EDG,BGD,BGD,BGD,CL2,BGD,BGD,EDG,
            EDG,BGD,BGD,BGD,CL1,BGD,BGD,EDG,EDG,BGD,BGD,CL2,BGD,BGD,BGD,EDG,
            EDG,BGD,BGD,BGD,CL1,BGD,BGD,EDG,EDG,BGD,CL2,BGD,BGD,BGD,BGD,EDG,
            EDG,BGD,BGD,CL1,CL1,CL1,BGD,EDG,EDG,BGD,CL2,CL2,CL2,CL2,BGD,EDG,
            CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,
            CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,
            EDG,BGD,CL3,CL3,CL3,BGD,BGD,EDG,EDG,BGD,CL4,BGD,CL4,BGD,BGD,EDG,
            EDG,BGD,BGD,BGD,BGD,CL3,BGD,EDG,EDG,BGD,CL4,BGD,CL4,BGD,BGD,EDG,
            EDG,BGD,CL3,CL3,CL3,BGD,BGD,EDG,EDG,BGD,CL4,BGD,CL4,BGD,BGD,EDG,
            EDG,BGD,BGD,BGD,BGD,CL3,BGD,EDG,EDG,BGD,CL4,CL4,CL4,CL4,BGD,EDG,
            EDG,BGD,BGD,BGD,BGD,CL3,BGD,EDG,EDG,BGD,BGD,BGD,CL4,BGD,BGD,EDG,
            EDG,BGD,CL3,CL3,CL3,BGD,BGD,EDG,EDG,BGD,BGD,BGD,CL4,BGD,BGD,EDG,
            CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,
            CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,
            EDG,BGD,CL5,CL5,CL5,CL5,BGD,EDG,EDG,BGD,BGD,CL6,CL6,CL6,BGD,EDG,
            EDG,BGD,CL5,BGD,BGD,BGD,BGD,EDG,EDG,BGD,CL6,BGD,BGD,BGD,BGD,EDG,
            EDG,BGD,CL5,CL5,CL5,BGD,BGD,EDG,EDG,BGD,CL6,CL6,CL6,BGD,BGD,EDG,
            EDG,BGD,BGD,BGD,BGD,CL5,BGD,EDG,EDG,BGD,CL6,BGD,BGD,CL6,BGD,EDG,
            EDG,BGD,BGD,BGD,BGD,CL5,BGD,EDG,EDG,BGD,CL6,BGD,BGD,CL6,BGD,EDG,
            EDG,BGD,CL5,CL5,CL5,BGD,BGD,EDG,EDG,BGD,BGD,CL6,CL6,BGD,BGD,EDG,
            CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,CNR,EDG,EDG,EDG,EDG,EDG,EDG,CNR,
            CNR,CL1,CL1,BGD,BGD,CL4,BGD,BGD,CL3,CL3,BGD,CL4,CL4,CL6,BGD,CNR,
            CNR,CL1,BGD,CL1,CL4,CL4,CL4,CL3,CL3,BGD,CL4,BGD,BGD,CL6,BGD,CNR,
            CNR,CL1,BGD,CL1,CL4,BGD,CL4,CL3,BGD,BGD,CL4,BGD,BGD,CL6,BGD,CNR,
            CNR,CL1,CL1,BGD,CL4,BGD,CL4,CL3,BGD,BGD,CL4,BGD,BGD,CL6,BGD,CNR,
            CNR,CL1,BGD,CL1,CL4,CL4,CL4,CL3,BGD,CL3,CL4,CL4,CL4,CL6,BGD,CNR,
            CNR,CL1,BGD,CL1,CL4,BGD,CL4,CL3,BGD,CL3,CL4,BGD,BGD,CL6,BGD,CNR,
            CNR,CL1,BGD,CL1,CL4,BGD,CL4,CL3,CL3,CL3,CL4,BGD,BGD,CL6,BGD,CNR,
            CNR,CL1,CL1,BGD,CL4,BGD,CL4,BGD,CL3,CL3,BGD,CL4,CL4,CL6,CL6,CNR
    };
    DebugCube::DebugCube() {
        hasInitialized = false;
        lightDir = glm::vec3(0.698535, 0.698535, -0.15523);
        lightColor = glm::vec3(0.15, 0.2, 0.1);
        mvp = glm::mat4(1.0f);
    }
    DebugCube::~DebugCube() {
    //    if (hasInitialized){
    //        glDeleteBuffers (4, buffers);
    //        glDeleteProgram (shdrLoc);
    //        glDeleteTextures (1, &texture);
    //        glDeleteVertexArrays (1, &VAOloc);
    //    }
    }
    bool DebugCube::Init(std::stringstream& output) {

        output << "\nBeginning initialization of debug cube.\n";

        // shader part
        shdrLoc = loadShaders("Assets/Shaders/debugCube.vert", "Assets/Shaders/debugCube.frag", output);

        attrLoc_verts   = glGetAttribLocation(shdrLoc, "Vertex");
        attrLoc_norms   = glGetAttribLocation(shdrLoc, "Normal");
        attrLoc_uvCoo   = glGetAttribLocation(shdrLoc, "UV");
        unifLoc_MVP     = glGetUniformLocation(shdrLoc, "MVP");
        unifLoc_M       = glGetUniformLocation(shdrLoc, "M");
        txtrLoc_diffuse = glGetUniformLocation(shdrLoc, "tex_diffuse");
        unifLoc_lightDir = glGetUniformLocation(shdrLoc, "lightDirection");
        unifLoc_lightClr = glGetUniformLocation(shdrLoc, "lightColor");

        // buffer part
        glGenVertexArrays(1, &VAOloc);
        glBindVertexArray(VAOloc);

        glGenBuffers(4, buffers);



        /*        FIX SIZEOF THING                FIX SIZEOF THING           DO IT NOW!       */



        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(attrLoc_verts);
        glVertexAttribPointer(attrLoc_verts, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof normals, normals, GL_STATIC_DRAW);
        glEnableVertexAttribArray(attrLoc_norms);
        glVertexAttribPointer(attrLoc_norms, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof uvCoords, uvCoords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(attrLoc_uvCoo);
        glVertexAttribPointer(attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, rawTexture);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        hasInitialized = true;
        output << "Debug cube has initialized.\n\n";
        return true;
    }
    void DebugCube::render(const glm::mat4& m, const glm::mat4& vp) {

		glBindVertexArray(VAOloc);

        mvp = vp * m;

        glActiveTexture (GL_TEXTURE0);
        glBindTexture (GL_TEXTURE_2D, texture);

        glUseProgram (shdrLoc);
        glUniformMatrix4fv (unifLoc_MVP, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv (unifLoc_M, 1, GL_FALSE, &m[0][0]);
        glUniform1i (txtrLoc_diffuse, 0);
        glUniform3fv(unifLoc_lightDir, 1, &lightDir[0]);
        glUniform3fv(unifLoc_lightClr, 1, &lightColor[0]);

        glDrawElements (GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
    }
}

