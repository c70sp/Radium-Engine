#version 410 core

in vec2 v_texCoord;

out vec4 color;

uniform vec4 u_color;
uniform sampler2D u_diffuse;

void main(){    
    vec4 texColor = texture(u_diffuse, v_texCoord);
    // ?! vvvvvv if textures look weird or something, idk vvvvvv
    // if(texColor.r < 0.1 && texColor.g < 0.1 && texColor.b < 0.1) {
    //     color = vec4(1.0, 0.0, 1.0, 0.0); // Magenta = texture is black
    // } else {
    //     color = texColor;
    // }
    color = texColor;
}