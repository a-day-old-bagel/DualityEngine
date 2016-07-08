#version 330
                                                                               
in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 WorldPos0;                                                                  
flat in int InstanceID;                                                             
                                                                                    
out vec4 FragColor;

const int MAX_POINT_LIGHTS = 2;                                                     
const int MAX_SPOT_LIGHTS = 2;

struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};
struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                                 
    vec3 Direction;                                                                 
};
struct Attenuation                                                                  
{                                                                                   
    float Constant;                                                                 
    float Linear;                                                                   
    float Exp;                                                                      
};
struct PointLight                                                                           
{                                                                                           
    BaseLight Base;                                                                         
    vec3 Position;                                                                          
    Attenuation Atten;                                                                      
};
struct SpotLight                                                                            
{                                                                                           
    PointLight Base;                                                                        
    vec3 Direction;                                                                         
    float Cutoff;                                                                           
};

uniform int gNumPointLights;                                                                
uniform int gNumSpotLights;                                                                 
uniform DirectionalLight gDirectionalLight;                                                 
uniform PointLight gPointLights[MAX_POINT_LIGHTS];                                          
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];                                             
uniform sampler2D gColorMap;                                                                
uniform vec3 gEyeWorldPos;                                                                  
uniform float gMatSpecularIntensity;                                                        
uniform float gSpecularPower;                                                               
uniform vec4 gColor[4]; 

void main()                                                                                 
{                                                    
    FragColor = vec4(Normal0, 1.0);
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}