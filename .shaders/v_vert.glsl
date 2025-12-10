#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 texCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_VP;

out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoords;

void main(){
    vTexCoords = texCoords;
    vec4 worldPos = u_ModelMatrix * vec4(position, 1.0);
    vFragPos = worldPos.xyz;
    vNormal = normalize(mat3(transpose(inverse(u_ModelMatrix))) * norm);
    gl_Position = u_VP * worldPos;
}