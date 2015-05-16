#version 130

in vec3 normal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D tex_diffuse;

void main()
{
    vec4 texSamp = texture(tex_diffuse, uv);
    float intensity = dot(normal, vec3(-0.57735, -0.57735, 0.57735));
    fragColor = vec4(texSamp.xyz * max(intensity, 0.4), 1.0);
    //fragColor = vec4(0.5 * normal + vec3(0.4, 0.4, 0.4), 1.0);
    //fragColor = vec4(0.4, 1.0, 0.0, 1.0);
}