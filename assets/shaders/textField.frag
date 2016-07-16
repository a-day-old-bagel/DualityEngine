#version 130

in vec2 uv;

out vec4 fragColor;

uniform sampler2D font_texture;
uniform vec3 penColor;

void main() {
    fragColor = vec4(penColor.r, penColor.g, penColor.b , texture(font_texture, uv).r);
//    fragColor = vec4(penColor.r, penColor.g, penColor.b , 1.0);
}