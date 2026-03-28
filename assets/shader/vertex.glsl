#version 410 core

layout(location = 0) in uint packed_data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 base_uv;
flat out uint dir;
flat out uint tex_id;
out float ambient_occlusion;

vec2 base_uvs[4] = vec2[4](
        vec2(0.0, 0.0),
        vec2(0.0, 1.0),
        vec2(1.0, 1.0),
        vec2(1.0, 0.0)
    );

float ambient_occlusion_multiplier[4] = float[4](
        1.0f,
        0.85f,
        0.70f,
        0.50f
    );

void main() {
    float x = float((packed_data >> 27u) & 0x1Fu);
    float y = float((packed_data >> 22u) & 0x1Fu);
    float z = float((packed_data >> 17u) & 0x1Fu);

    uint texture_id = (packed_data >> 9u) & 0xFFu;
    uint direction = (packed_data >> 6u) & 0x7u;
    uint corner = (packed_data >> 4u) & 0x3u;
    uint ambient_occl = (packed_data >> 2u) & 0x3u;

    gl_Position = projection * view * model * vec4(x - 0.5, y - 0.5, z - 0.5, 1.0);

    base_uv = base_uvs[corner];
    dir = direction;
    tex_id = texture_id;
    ambient_occlusion = ambient_occlusion_multiplier[ambient_occl];
}
