#version 130

in vec3 normal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D tex_diffuse;
uniform vec3 lightDirection;

void main()
{
    vec4 texSamp = texture(tex_diffuse, uv);
    float intensity = dot(normal, lightDirection);
    fragColor = vec4(texSamp.xyz * max(intensity, 0.4), 1.0) + vec4(vec3(0.15, 0.1, 0.02) * intensity, 1.0);
}