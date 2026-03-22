#pragma once
#ifndef VOXIDE_BLOCK_HEADER
#define VOXIDE_BLOCK_HEADER

#include <cstdint>

enum class Block : std::uint8_t {
    Air = 0,
    Grass = 1,
    Stone = 2,
};

#endif // VOXIDE_BLOCK_HEADER
