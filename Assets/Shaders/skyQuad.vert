#version 130

uniform mat4 projMat;
uniform mat4 viewMat;

in vec4 Vertex;

out vec3 eyeDirection;

void main() {
    mat4 inverseProjection = inverse(projMat);
    mat3 inverseModelview = transpose(mat3(viewMat));
    vec3 unprojected = (inverseProjection * Vertex).xyz;
    eyeDirection = inverseModelview * unprojected;

    gl_Position = Vertex;
}