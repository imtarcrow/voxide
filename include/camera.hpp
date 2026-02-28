#pragma once
#ifndef VOXIDE_CAMERA_HEADER
#define VOXIDE_CAMERA_HEADER

#include <glm/glm.hpp>

constexpr glm::vec3 WORLD_UP = { 0.0, 1.0, 0.0 };

class Camera
{

private:
    glm::vec3 position;

    glm::vec3 up = WORLD_UP;
    glm::vec3 front = { 0.0F, 0.0F, -1.0F };
    glm::vec3 right = { 1.0F, 0.0F, 0.0F };

    float yaw = -90.0F;
    float pitch = 0;

    float fovy;
    float aspect_ratio;

    void update_direction_vectors();

public:
    Camera(float fovy, glm::vec3 position, float aspect_ratio);
    ~Camera() = default;

    // disable copying
    Camera(const Camera&) = delete;
    auto operator=(const Camera&) -> Camera& = delete;

    // Enable cheap moving
    Camera(Camera&&) noexcept = default;
    auto operator=(Camera&&) noexcept -> Camera& = default;

    void set_position(glm::vec3 position) noexcept;
    void set_x(float xpos) noexcept;
    void set_y(float ypos) noexcept;
    void set_z(float zpos) noexcept;

    void set_angles(float yaw, float pitch) noexcept;
    void set_yaw(float yaw) noexcept;
    void set_pitch(float pitch) noexcept;

    void set_aspect_ratio(float aspect_ratio) noexcept;

    [[nodiscard]] auto get_position() const noexcept -> glm::vec3;
    [[nodiscard]] auto get_x() const noexcept -> float;
    [[nodiscard]] auto get_y() const noexcept -> float;
    [[nodiscard]] auto get_z() const noexcept -> float;

    [[nodiscard]] auto get_angles() const noexcept -> std::pair<float, float>;
    [[nodiscard]] auto get_yaw() const noexcept -> float;
    [[nodiscard]] auto get_pitch() const noexcept -> float;

    [[nodiscard]] auto get_aspect_ratio() const noexcept -> float;

    [[nodiscard]] auto get_view_matrix() const noexcept -> glm::mat4;
    [[nodiscard]] auto get_projection_matrix() const noexcept -> glm::mat4;
};

#endif // !VOXIDE_CAMERA_HEADER
