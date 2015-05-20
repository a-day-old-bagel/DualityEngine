#version 130

in vec2 Vertex;
//in vec2 UV;

//out vec2 uv;

void main(){

    //uv = UV;
    gl_Position = vec4(Vertex, -1.0, 1.0);
}

