#version 150

uniform samplerCube cubeTexture;

in vec3 eyeDirection;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(cubeTexture, eyeDirection);
}