#include "chunk.hpp"

#include <glm/fwd.hpp>
#include <print>
#include <spdlog/spdlog.h>
#include <stdexcept>

#include "glad/glad.h"

Chunk::Chunk(glm::ivec3 position)
    : position(position)
{
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glGenVertexArrays(1, &this->VAO);

    if (this->VBO == GL_INVALID_VALUE || this->EBO == GL_INVALID_VALUE) {
        spdlog::error("glGenBuffers returned GL_INVALID_VALUE");
        throw std::runtime_error("glGenBuffers returned GL_INVALID_VALUE");
    }

    if (this->VAO == GL_INVALID_VALUE) {
        spdlog::error("glGenVertexArrays returned GL_INVALID_VALUE");
        throw std::runtime_error("glGenVertexArrays returned GL_INVALID_VALUE");
    }

    this->blocks.fill(1);
    this->gen_chunk_mesh();
}

Chunk::~Chunk()
{
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
    glDeleteVertexArrays(1, &this->VAO);
}

auto Chunk::is_in_chunk(glm::ivec3 position) -> bool
{
    if (position.x < 0 || position.x > 15) {
        return false;
    }

    if (position.y < 0 || position.y > 15) {
        return false;
    }

    if (position.z < 0 || position.z > 15) {
        return false;
    }

    return true;
}

auto Chunk::get_block_at(glm::ivec3 position) -> char
{
    if (!this->is_in_chunk(position)) {
        return 0;
    }

    return this->blocks[((position.x * 15 * 15) + (position.y * 15) + position.z)];
}

void Chunk::gen_chunk_mesh()
{
    std::vector<float> verticies = {};
    std::vector<unsigned int> indicies = {};

    for (int ypos = 0; ypos < 16; ypos++) {
        for (int zpos = 0; zpos < 16; zpos++) {
            for (int xpos = 0; xpos < 16; xpos++) {
                if (this->get_block_at({ xpos, ypos, zpos }) == 0) {
                    continue;
                }

                // X+ facing
                if (this->get_block_at({ xpos + 1, ypos, zpos }) == 0) {
                    spdlog::trace("PUSCH X+ {} {} {}", xpos, ypos, zpos);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 1);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 3);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);
                }
                
                //X- facing
                if (this->get_block_at({ xpos - 1, ypos, zpos }) == 0) {
                    spdlog::trace("PUSCH X- {} {} {}", xpos, ypos, zpos);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 1);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 3);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);
                }

                //Z+ facing
                if (this->get_block_at({ xpos, ypos, zpos + 1}) == 0) {
                    spdlog::trace("PUSCH Z+ {} {} {}", xpos, ypos, zpos);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 1);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 3);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);
                }

                //Z- facing
                if (this->get_block_at({ xpos, ypos, zpos - 1 }) == 0) {
                    spdlog::trace("PUSCH Z- {} {} {}", xpos, ypos, zpos);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 1);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 3);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);
                }  

                //Y+ facing
                if (this->get_block_at({ xpos, ypos + 1, zpos}) == 0) {
                    spdlog::trace("PUSCH Y+ {} {} {}", xpos, ypos, zpos);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 1);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 3);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) + 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);
                }  
                
                //Y- facing
                if (this->get_block_at({ xpos, ypos - 1, zpos}) == 0) {
                    spdlog::trace("PUSCH Y- {} {} {}", xpos, ypos, zpos);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 1);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 0);
                    indicies.push_back((verticies.size() / 3) + 2);
                    indicies.push_back((verticies.size() / 3) + 3);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) + 0.5F);

                    verticies.push_back(static_cast<float>(xpos) - 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);

                    verticies.push_back(static_cast<float>(xpos) + 0.5F);
                    verticies.push_back(static_cast<float>(ypos) - 0.5F);
                    verticies.push_back(static_cast<float>(zpos) - 0.5F);
                }  
            }
        }
    }

    std::println("Verticies: ");
    for (auto& vert : verticies) {
        std::print("{} ", vert);
    }
    std::println("");
    std::println("");

    std::println("Indicies: ");
    for (auto& inds : indicies) {
        std::print("{} ", inds);
    }
    std::println("");
    std::println("");

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), indicies.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    this->indicies_size = indicies.size();
    spdlog::trace("size indicies: {}", this->indicies_size);
}
