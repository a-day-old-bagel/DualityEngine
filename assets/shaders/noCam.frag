#version 130

uniform sampler2D tex;

in vec2 texPosition;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(tex, texPosition);
}