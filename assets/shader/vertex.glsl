#version 410 core

layout(location = 0) in uint data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vcolor;

void main() {
    float x = float((data >> 0u) & 0x1Fu);
    float y = float((data >> 5u) & 0x1Fu);
    float z = float((data >> 10u) & 0x1Fu);
    uint dir = (data >> 15u) & 0x7u;
    uint texture = (data >> 18u) & 0x3Fu;

    gl_Position = projection * view * model * vec4(x, y, z, 1.0);

    if (dir == 0u) vcolor = vec3(1.0, 0.0, 0.0); // X+
    else if (dir == 1u) vcolor = vec3(0.5, 0.0, 0.0); // X-
    else if (dir == 2u) vcolor = vec3(0.0, 0.0, 1.0); // Z+
    else if (dir == 3u) vcolor = vec3(0.0, 0.0, 0.5); // Z-
    else if (dir == 4u) vcolor = vec3(0.0, 1.0, 0.0); // Y+
    else vcolor = vec3(0.0, 0.5, 0.0); // Y-
}
