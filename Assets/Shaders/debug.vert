#version 330

layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;                                               
layout (location = 3) in mat4 WVP;                                                  
layout (location = 7) in mat4 World;                                                
                                                               
out vec3 Normal0;                                                           
                                                                                    
void main()                                                                         
{                                                                                   
    gl_Position = WVP * vec4(Position, 1.0);                                                      
    Normal0     = (World * vec4(Normal, 0.0)).xyz;
    //gl_Position = vec4(.5,.5,.5,1);
}