#version 130
//#define SPACE

in vec3 normal;
in vec2 uv;

out vec4 fragColor;

#ifdef SPACE
vec3 ambientColor = vec3(1.0, 1.0, 1.0);
vec3 diffuseColor = vec3(1.0, 0.8, 0.6);
vec3 lightDirection = vec3 (0.0, 0.1, 1.0);
#endif

void main()
{
    vec3 norm = normalize(normal);
    #ifdef SPACE
    float ambientIntensity = 0.1;
    float diffuseIntensity = max(0.0, dot(norm, lightDirection));
    float red   = diffuseIntensity * diffuseColor.r + ambientIntensity * ambientColor.r;
    float green = diffuseIntensity * diffuseColor.g + ambientIntensity * ambientColor.g;
    float blue  = diffuseIntensity * diffuseColor.b + ambientIntensity * ambientColor.b;
    #else
    float red   = (dot(norm, vec3(1.0, 0.0, 0.0)) + 1.0) * 0.5;
    float green = (dot(norm, vec3(0.0, 1.0, 0.0)) + 1.0) * 0.5;
    float blue  = (dot(norm, vec3(1.0, 0.0, 1.0)) + 1.0) * 0.5;
    #endif
    fragColor = vec4(red, green, blue, 1.0);
}