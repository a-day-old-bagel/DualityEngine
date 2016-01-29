#version 130

in vec3 normal;
in vec2 uv;

out vec4 fragColor;

void main()
{
    fragColor = vec4(normal.x, normal.y, normal.z, 1.0);
    //fragColor = vec4(1.0, 0.0, 1.0, 1.0);
}