#version 410 core

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoords;

uniform sampler2D u_texture;

out vec4 color;

void main(){
    vec3 sampled = vec3(texture(u_texture, vTexCoords)); //texture(u_texture, vTexCoords)

    vec3 lightPos = vec3(100.0, 0.0, 500.0);
    vec3 baseColor = sampled;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * vec3(0.7);
    vec3 ambient = vec3(0.5);

    vec3 light = diffuse + ambient;
    vec3 brightness = baseColor * light;

    // vec4 tempColor = texture(u_texture, vTexCoords);
    // color = tempColor * vec4(brightness, 1.0);
    // color = texture(u_texture, vTexCoords);
    color = vec4(1.0);
}