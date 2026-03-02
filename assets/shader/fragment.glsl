#version 410 core

out vec4 FragColor;

in vec3 vcolor;

void main() {
    FragColor = vec4(vcolor, 1.0);
}
