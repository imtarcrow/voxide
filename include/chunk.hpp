#pragma once
#include "shader_program.hpp"
#ifndef VOXIDE_CHUNK_HEADER
#define VOXIDE_CHUNK_HEADER

#include <cstddef>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <memory>

#include "chunk_mesh.hpp"

constexpr std::size_t CHUNK_SIZE_X = 16;
constexpr std::size_t CHUNK_SIZE_Y = 16;
constexpr std::size_t CHUNK_SIZE_Z = 16;
constexpr std::size_t CHUNK_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

class Chunk
{

private:
    std::array<std::uint8_t, CHUNK_SIZE> blocks = { 0 };

    glm::ivec3 position;
    std::unique_ptr<ChunkMesh> mesh;

    [[nodiscard]] auto validate_coordinates(glm::ivec3 position) const noexcept -> bool;

public:
    int indicies_size = 0;

    Chunk(glm::ivec3 position);
    ~Chunk() = default;

    // disable copying
    Chunk(const Chunk&) = delete;
    auto operator=(const Chunk&) -> Chunk& = delete;

    // enable moving
    Chunk(Chunk&&) noexcept = default;
    auto operator=(Chunk&&) noexcept -> Chunk& = default;

    void render(ShaderProgram& program) const noexcept;

    [[nodiscard]] auto get_block_at(glm::ivec3 position) const noexcept -> std::uint8_t;
    void set_block_at(glm::ivec3 position, std::uint8_t block_id) noexcept;

    [[nodiscard]] auto get_position() const noexcept -> glm::ivec3;
    void set_position(glm::ivec3 position) noexcept;
};

#endif // VOXIDE_CHUNK_HEADER
