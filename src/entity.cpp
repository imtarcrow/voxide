#include "entity.hpp"

auto Entity::get_id() const noexcept -> std::uint64_t
{
    return this->entity_id;
}

auto Entity::get_position() const noexcept -> glm::vec3
{
    return this->position;
}

void Entity::set_position(glm::vec3 position) noexcept
{
    this->position = position;
}

auto Entity::get_yaw() const noexcept -> float
{
    return this->yaw;
}

void Entity::set_yaw(float yaw) noexcept
{
    this->yaw = yaw;
}

auto Entity::get_pitch() const noexcept -> float
{
    return this->pitch;
}

void Entity::set_pitch(float pitch) noexcept
{
    this->pitch = pitch;
}
