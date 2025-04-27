#version 430 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTex;

uniform mat4 model;
uniform mat4 model_T_inv;
uniform mat4 view;
uniform mat4 project;

out vec3 FlagPos;
out vec3 FlagNormal;
out vec2 Texcoord;

void main()
{
    FlagPos = vec3(model * vec4(aPos,1.0));
    gl_Position = project * view * vec4(FlagPos,1.0);
    FlagNormal = mat3(model_T_inv) * aNormal;
    FlagNormal = normalize(FlagNormal);
    Texcoord = aTex;
}