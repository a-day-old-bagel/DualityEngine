#version 130

uniform sampler2D tex;

smooth in vec2 texPosition;

out vec4 fragmentColor;

void main() {
    //fragmentColor = texture(tex, texPosition);
    fragmentColor = vec4(1.0,1.0,1.0,1.0);
}