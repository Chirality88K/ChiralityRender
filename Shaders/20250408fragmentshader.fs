#version 430 core
in vec3 FlagPos;
in vec3 FlagNormal;
in vec2 Texcoord;
out vec4 Fragcolor;

struct ParallelLight{
    vec3 Direction;
    vec3 Color;
};

struct Material{
    vec4 Diffuse;
	vec4 Ambient;
    vec4 Specular;
	float Shininess;
};

uniform vec3 CameraPos;
uniform ParallelLight pLight0;
uniform Material material0;
uniform sampler2D ourTexture;

void main(){
    vec4 ambient = material0.Ambient*vec4(pLight0.Color,1.0)*0.4;
    vec3 L = -pLight0.Direction;
    vec4 diffuse = material0.Diffuse*vec4(pLight0.Color,1.0)*max(0.0,dot(L,FlagNormal));
    vec3 v = normalize(CameraPos-FlagPos);
    vec3 h = normalize(v+L);
    vec4 specular = material0.Specular*vec4(pLight0.Color,1.0)*pow(max(0.0,dot(h,FlagNormal)),material0.Shininess);
    Fragcolor = (ambient+diffuse+specular)*texture(ourTexture,Texcoord);
}