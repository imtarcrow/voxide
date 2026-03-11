#version 410 core

layout(location = 0) in uint packed_data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 base_uv;
flat out uint dir;

void main() {

    float x = float((packed_data >> 27u) & 0x1Fu);
    float y = float((packed_data >> 22u) & 0x1Fu);
    float z = float((packed_data >> 17u) & 0x1Fu);

    uint texture_id = (packed_data >> 8u) & 0xFFu;
    uint direction = (packed_data >> 6u) & 0x7u;
    uint corner = (packed_data >> 4u) & 0x3u; 
    uint ambient_occlusion = (packed_data >> 2u) & 0x3u;

    gl_Position = projection * view * model * vec4(x - 0.5, y - 0.5, z - 0.5, 1.0);

    if(corner == 0) base_uv = vec2(0.0, 0.0);
    if(corner == 1) base_uv = vec2(0.0, 1.0);
    if(corner == 2) base_uv = vec2(1.0, 1.0);
    if(corner == 3) base_uv = vec2(1.0, 0.0);

    dir = direction;

    // if (direction == 0u) vcolor = vec3(1.0, 0.0, 0.0); // X+
    // else if (direction == 1u) vcolor = vec3(0.5, 0.0, 0.0); // X-
    // else if (direction == 2u) vcolor = vec3(0.0, 1.0, 0.0); // Y+
    // else if (direction == 3u) vcolor = vec3(0.0, 0.5, 0.0); // Y-
    // else if (direction == 4u) vcolor = vec3(0.0, 0.0, 1.0); // Z+
    // else if (direction == 5u) vcolor = vec3(0.0, 0.0, 0.5); // Z-
}
