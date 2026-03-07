#include "chunk.hpp"

#include <FastNoiseLite.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "chunk_mesh.hpp"

Chunk::Chunk(glm::ivec3 position)
    : position(position)
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(1337);
    noise.SetFrequency(0.04F);

    for (int xpos = 0; xpos < CHUNK_SIZE_X; xpos++) {
        for (int zpos = 0; zpos < CHUNK_SIZE_Z; zpos++) {
            float value = noise.GetNoise(static_cast<float>(xpos + (position.x * 16)), static_cast<float>(zpos + (position.z * 16)));
            value = ((value + 1.0F) / 2.0F) * 50;

            int height = static_cast<int>(value);

            for (int ypos = 0; ypos < CHUNK_SIZE_Y; ypos++) {

                if (ypos + (position.y * CHUNK_SIZE_Y) < height)
                    this->set_block_at({ xpos, ypos, zpos }, 1);
            }
        }
    }

    // spdlog::debug("Created Chunk at {} {} {}", position.x, position.y, position.z);

    this->mesh = std::make_unique<ChunkMesh>();
    this->mesh->generate(*this);
}

auto Chunk::validate_coordinates(glm::ivec3 position) const noexcept -> bool
{
    return position.x >= 0 && position.x < static_cast<int>(CHUNK_SIZE_X) && position.y >= 0 && position.y < static_cast<int>(CHUNK_SIZE_Y)
        && position.z >= 0 && position.z < static_cast<int>(CHUNK_SIZE_Z);
}

void Chunk::render() const noexcept
{
    this->mesh->render();
}

auto Chunk::get_block_at(glm::ivec3 position) const noexcept -> std::uint8_t
{
    if (!this->validate_coordinates(position)) {
        return 0;
    }

    return this->blocks[(position.x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (position.y * CHUNK_SIZE_Z) + position.z];
}

void Chunk::set_block_at(glm::ivec3 position, std::uint8_t block_id) noexcept
{
    this->blocks[(position.x * CHUNK_SIZE_Y * CHUNK_SIZE_Z) + (position.y * CHUNK_SIZE_Z) + position.z] = block_id;
}

auto Chunk::get_position() const noexcept -> glm::ivec3
{
    return this->position;
}

void Chunk::set_position(glm::ivec3 position) noexcept
{
    this->position = position;
}

auto Chunk::calculate_chunk_key(glm::ivec3 position) noexcept -> std::uint64_t
{ // Bias to make all values positive
    auto ux = static_cast<uint64_t>(position.x + 0x800000); // 24 bit bias
    auto uy = static_cast<uint64_t>(position.y + 0x8000); // 16 bit bias
    auto uz = static_cast<uint64_t>(position.z + 0x800000); // 24 bit bias

    return (ux << 40U) | (uy << 24U) | uz;
}

auto Chunk::get_chunk_key() const noexcept -> std::uint64_t
{
    return Chunk::calculate_chunk_key(this->position);
}
