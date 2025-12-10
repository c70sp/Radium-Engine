#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

// uniform mat4 u_ModelMatrix;
uniform mat4 u_VP;

out vec2 v_texCoord;

void main(){
    v_texCoord = texCoord;

    // vec4 worldPos = u_ModelMatrix * vec4(position, 1.0);
    gl_Position = u_VP * vec4(position, 1.0);
}