#version 130

in vec4 Vertex;

smooth out vec2 texPosition;

void main() {
    
    texPosition = Vertex.xy;
    
    gl_Position = Vertex;
}