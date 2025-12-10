#version 410 core

layout(location = 0) in vec4 position;

uniform mat4 u_VP;

out vec2 TexCoords;

void main() {
    gl_Position = u_VP * vec4(position.xy, 0.0, 1.0);
    TexCoords = position.zw;
}