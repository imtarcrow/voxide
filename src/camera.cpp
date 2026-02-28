#include "camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

Camera::Camera(float fovy, glm::vec3 position, float aspect_ratio)
    : position(position)
    , fovy(fovy)
    , aspect_ratio(aspect_ratio)
{
    this->update_direction_vectors();
}

void Camera::update_direction_vectors()
{
    glm::vec3 new_front;

    new_front.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
    new_front.y = std::sin(glm::radians(this->pitch));
    new_front.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));

    this->front = glm::normalize(new_front);
    this->right = glm::normalize(glm::cross(this->front, WORLD_UP));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::set_position(glm::vec3 position) noexcept
{
    this->position = position;
}

void Camera::set_x(float xpos) noexcept
{
    this->position.x = xpos;
}

void Camera::set_y(float ypos) noexcept
{
    this->position.y = ypos;
}

void Camera::set_z(float zpos) noexcept
{
    this->position.z = zpos;
}

void Camera::set_angles(float yaw, float pitch) noexcept
{
    this->yaw = yaw;
    this->pitch = pitch;
    this->update_direction_vectors();
}

void Camera::set_yaw(float yaw) noexcept
{
    this->yaw = yaw;
    this->update_direction_vectors();
}

void Camera::set_pitch(float pitch) noexcept
{
    this->pitch = pitch;
    this->update_direction_vectors();
}

void Camera::set_aspect_ratio(float aspect_ratio) noexcept
{
    this->aspect_ratio = aspect_ratio;
}

auto Camera::get_position() const noexcept -> glm::vec3
{
    return this->position;
}

auto Camera::get_x() const noexcept -> float
{
    return this->position.x;
}

auto Camera::get_y() const noexcept -> float
{
    return this->position.y;
}

auto Camera::get_z() const noexcept -> float
{
    return this->position.z;
}

auto Camera::get_angles() const noexcept -> std::pair<float, float>
{
    return { this->yaw, this->pitch };
}

auto Camera::get_yaw() const noexcept -> float
{
    return this->yaw;
}

auto Camera::get_pitch() const noexcept -> float
{
    return this->pitch;
}

auto Camera::get_aspect_ratio() const noexcept -> float
{
    return this->aspect_ratio;
}

auto Camera::get_view_matrix() const noexcept -> glm::mat4
{
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

auto Camera::get_projection_matrix() const noexcept -> glm::mat4
{
    return glm::perspective(glm::radians(this->fovy), this->aspect_ratio, 0.01F, 1000.0F);
}
