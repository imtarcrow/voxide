#include "camera.hpp"

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

Camera::Camera(glm::vec3 position, float yaw, float pitch, float fovy, float aspect_ratio) noexcept
    : position(position)
    , yaw(yaw)
    , pitch(pitch)
    , fovy(fovy)
    , aspect_ratio(aspect_ratio)
{
    spdlog::trace("Creating Camera: \n > x={}, y={}, z={}\n > yaw={}, pitch={}\n > fovy={}\n > aspect-ratio={}", position.x, position.y,
                  position.z, yaw, pitch, fovy, aspect_ratio);

    this->update_direction_vectors();
}

Camera::~Camera()
{
    spdlog::trace("Destroying Camera");
}

void Camera::update_direction_vectors() noexcept
{
    glm::vec3 new_front_vector;

    new_front_vector.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
    new_front_vector.y = std::sin(glm::radians(this->pitch));
    new_front_vector.z = std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));

    this->front = glm::normalize(new_front_vector);
    this->right = glm::normalize(glm::cross(this->front, WORLD_UP));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::handle_look_input(float xoffset, float yoffset, bool constrain_pitch = true) noexcept
{
    xoffset *= this->look_sensitivity;
    yoffset *= this->look_sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if (constrain_pitch) {
        this->pitch = std::min(this->pitch, 89.999F);
        this->pitch = std::max(this->pitch, -89.999F);
    }

    this->yaw = std::fmod(this->yaw, 360.0F);

    this->update_direction_vectors();
}

void Camera::set_position(glm::vec3 position) noexcept
{
    this->position = position;
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

void Camera::set_look_sensitivity(float look_sensitivity) noexcept
{
    this->look_sensitivity = look_sensitivity;
}

auto Camera::get_position() const noexcept -> glm::vec3
{
    return this->position;
}

auto Camera::get_yaw() const noexcept -> float
{
    return this->yaw;
}

auto Camera::get_pitch() const noexcept -> float
{
    return this->pitch;
}

auto Camera::get_front_vector() const noexcept -> glm::vec3
{
    return this->front;
}

auto Camera::get_up_vector() const noexcept -> glm::vec3
{
    return this->up;
}

auto Camera::get_right_vector() const noexcept -> glm::vec3
{
    return this->right;
}

auto Camera::get_aspect_ratio() const noexcept -> float
{
    return this->aspect_ratio;
}

auto Camera::get_look_sensitivity() const noexcept -> float
{
    return this->look_sensitivity;
}

auto Camera::get_view_matrix() const noexcept -> glm::mat4
{
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

auto Camera::get_projection_matrix() const noexcept -> glm::mat4
{
    return glm::perspective(glm::radians(this->fovy), this->aspect_ratio, NEAR_PLANE, FAR_PLANE);
}
