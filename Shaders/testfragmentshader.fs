#version 430 core
in vec3 FlagPos;
in vec3 FlagNormal;
in vec2 Texcoord;
out vec4 Fragcolor;

struct PointLight{
    vec3 Position;
    vec3 Color;

    float Constant;
    float Linear;
    float Quadratic;
};

struct Material{
    vec3 DiffuseAlbedo;
	vec3 FresnelR0;
	float Roughness;
};

uniform vec3 CameraPos;
uniform PointLight pLight0;
uniform Material material0;
uniform sampler2D ourTexture;

void main(){
    vec3 ambient = material0.DiffuseAlbedo*pLight0.Color*0.3;
    vec3 L = normalize(pLight0.Position-FlagPos);
    vec3 diffuse = material0.DiffuseAlbedo*pLight0.Color*max(0.0,dot(L,FlagNormal));
    vec3 v = normalize(CameraPos-FlagPos);
    vec3 h = normalize(v+L);
    vec3 Fresnel = material0.FresnelR0+(vec3(1.0,1.0,1.0)-material0.FresnelR0)*pow((1-dot(h,L)),5);
    int m = int(material0.Roughness*256.0);
    float s = (float(m)/8.0+1.0)*pow(dot(FlagNormal,h),m);
    vec3 specular = material0.DiffuseAlbedo*pLight0.Color*max(0.0,dot(L,FlagNormal))*s*Fresnel;
    float distance = length(pLight0.Position-FlagPos);
    float attenuation = 1.0/(pLight0.Constant+distance*pLight0.Linear+distance*distance*pLight0.Quadratic);
    Fragcolor = vec4((ambient+diffuse+specular)*attenuation,1.0)*texture(ourTexture,Texcoord);
}