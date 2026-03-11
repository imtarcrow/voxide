#version 410 core

in vec2 base_uv;
flat in uint dir;

uniform sampler2D texture_atlas;

out vec4 FragColor;

void main() {
    if (dir % 2 == 0) {
        FragColor = texture(texture_atlas, base_uv);
    } else {
        FragColor = texture(texture_atlas, base_uv) * 0.8;
    }
}
