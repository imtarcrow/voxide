#version 410 core

in vec2 base_uv;
flat in uint dir;
flat in uint tex_id;

uniform sampler2D texture_atlas;

out vec4 FragColor;

vec3 dirHints[6] = vec3[6](
        vec3(1, 0, 0), // +X
        vec3(0, 1, 1), // -X
        vec3(0, 1, 0), // +Y
        vec3(1, 0, 1), // -Y
        vec3(0, 0, 1), // +Z
        vec3(1, 1, 0) // -Z
    );

void main()
{
    float tiles_per_row = float(textureSize(texture_atlas, 0).x) / 16.0;
    float tile_uv_size  = 1.0 / tiles_per_row;

    float col = mod(float(tex_id), tiles_per_row);
    float row = floor(float(tex_id) / tiles_per_row);

    vec2 tile_origin = vec2(col, row) * tile_uv_size;

    float half_texel = 0.5 / float(textureSize(texture_atlas, 0).x);
    vec2 clamped_uv = clamp(base_uv, 0.0, 1.0 - (1.0 / 16.0));
    vec2 uv = tile_origin + clamped_uv * tile_uv_size;

    // clamp uv to tile bounds with half-texel inset
    vec2 tile_min = tile_origin + half_texel;
    vec2 tile_max = tile_origin + tile_uv_size - half_texel;
    uv = clamp(uv, tile_min, tile_max);

    FragColor = texture(texture_atlas, uv);
}
