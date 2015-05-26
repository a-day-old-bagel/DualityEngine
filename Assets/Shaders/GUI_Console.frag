#version 130

in vec2 uv;

out vec4 fragColor;

uniform sampler2D font_texture;
uniform vec3 penColor;

void main()
{
    //vec2 texSamp = texture(font_texture, uv);
    //fragColor = vec4(1.0, 1.0, 1.0, texSamp.r);
    //fragColor = vec4(charTexDimensions.x / 10, charTexDimensions.y / 10, 1.0, 1.0);

    fragColor = vec4(penColor.r, penColor.g, penColor.b , texture(font_texture, uv).r);
    //vec3 colors = texture(font_texture, uv).rgb
    //fragColor = vec4(colors.r, colors.g, colors.b , 0.5);
}