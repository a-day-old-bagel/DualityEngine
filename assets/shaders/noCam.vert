#version 130

in vec4 Vertex;

out vec2 texPosition;

void main() {
    texPosition = vec2( (Vertex.x + 1.0) * 0.5, (Vertex.y - 1.0) * -0.5 );
    gl_Position = Vertex;
}