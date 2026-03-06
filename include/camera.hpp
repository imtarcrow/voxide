#pragma once
#ifndef VOXIDE_CAMERA_HEADER
#define VOXIDE_CAMERA_HEADER

#include <glm/glm.hpp>

constexpr glm::vec3 WORLD_UP = { 0.0F, 1.0F, 0.0F };

constexpr float DEFAULT_YAW = -90.0F;
constexpr float DEFAULT_PITCH = 0.0F;
constexpr float DEFAULT_FOVY = 90.0F;
constexpr float DEFAULT_ASPECT_RATIO = 16.0F / 9.0F;

constexpr float NEAR_PLANE = 0.01F;
constexpr float FAR_PLANE = 1000.0F;

class Camera
{

private:
    glm::vec3 position;

    glm::vec3 up = WORLD_UP;
    glm::vec3 front = { 0.0F, 0.0F, -1.0F };
    glm::vec3 right = { 1.0F, 0.0F, 0.0F };

    float yaw;
    float pitch;

    float mouse_sensitivity = 0.1F;

    float fovy;
    float aspect_ratio;

    void update_direction_vectors();

public:
    explicit Camera(glm::vec3 position, glm::vec2 angles = { DEFAULT_YAW, DEFAULT_PITCH }, float fovy = DEFAULT_FOVY,
                    float aspect_ratio = DEFAULT_ASPECT_RATIO) noexcept;
    explicit Camera(float xpos, float ypos, float zpos, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH, float fovy = DEFAULT_FOVY,
                    float aspect_ratio = DEFAULT_ASPECT_RATIO) noexcept;

    Camera() = delete;
    ~Camera();

    // disable copying
    Camera(const Camera&) = delete;
    auto operator=(const Camera&) -> Camera& = delete;

    // Enable cheap moving
    Camera(Camera&&) noexcept = default;
    auto operator=(Camera&&) noexcept -> Camera& = default;

    void process_mouse_movement(float xoffset, float yoffset, bool constrain_pitch) noexcept;

    void set_position(glm::vec3 position) noexcept;
    void set_x(float xpos) noexcept;
    void set_y(float ypos) noexcept;
    void set_z(float zpos) noexcept;

    void set_angles(float yaw, float pitch) noexcept;
    void set_yaw(float yaw) noexcept;
    void set_pitch(float pitch) noexcept;

    void set_aspect_ratio(float aspect_ratio) noexcept;

    void set_mouse_sensitivity(float sensitivity) noexcept;

    [[nodiscard]] auto get_position() const noexcept -> glm::vec3;
    [[nodiscard]] auto get_x() const noexcept -> float;
    [[nodiscard]] auto get_y() const noexcept -> float;
    [[nodiscard]] auto get_z() const noexcept -> float;

    [[nodiscard]] auto get_angles() const noexcept -> glm::vec2;
    [[nodiscard]] auto get_yaw() const noexcept -> float;
    [[nodiscard]] auto get_pitch() const noexcept -> float;

    [[nodiscard]] auto get_front_vector() const noexcept -> glm::vec3;
    [[nodiscard]] auto get_up_vector() const noexcept -> glm::vec3;
    [[nodiscard]] auto get_right_vector() const noexcept -> glm::vec3;

    [[nodiscard]] auto get_aspect_ratio() const noexcept -> float;

    [[nodiscard]] auto get_mouse_sensitivity() const noexcept -> float;

    [[nodiscard]] auto get_view_matrix() const noexcept -> glm::mat4;
    [[nodiscard]] auto get_projection_matrix() const noexcept -> glm::mat4;
};

#endif // !VOXIDE_CAMERA_HEADER
