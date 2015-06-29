#version 130

uniform samplerCube cubeTexture;

in vec3 eyeDirection;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(cubeTexture, eyeDirection);//vec4(1.0, 1.0, 1.0, 1.0);//
}