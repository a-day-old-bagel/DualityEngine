#version 130

in vec3 Vertex;
in vec2 UV;
out vec2 uv;
uniform mat4 MVP;

void main() {
    uv = UV;
    gl_Position = MVP * vec4(Vertex, 1.0);
}
