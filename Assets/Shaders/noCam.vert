#version 130

in vec4 Vertex;

out vec2 texPosition;

void main() {
    
    texPosition = vec2(Vertex.x * 4 + 0.3, -Vertex.y * 4);
    
    gl_Position = Vertex;
}