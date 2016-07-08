#version 130

in vec3 Vertex;
in vec3 Normal;
in vec2 UV;

out vec3 normal;
out vec2 uv;

uniform mat4 M;
uniform mat4 MVP;

void main(){

    normal = (M * vec4(Normal, 0.0)).xyz;
    uv = UV;
    gl_Position = MVP * vec4(Vertex, 1.0);

}

