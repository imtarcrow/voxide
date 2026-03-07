#pragma once
#ifndef VOXIDE_CAMERA_HEADER
#define VOXIDE_CAMERA_HEADER

#include <glm/glm.hpp>

constexpr glm::vec3 WORLD_UP = { 0.0F, 1.0F, 0.0F };
constexpr glm::vec3 DEFAULT_FRONT = {0.0F, 0.0F, -1.0F};
constexpr glm::vec3 DEFAULT_RIGHT = {1.0F, 0.0F, 0.0F};

constexpr float DEFAULT_YAW = -90.0F;
constexpr float DEFAULT_PITCH = 0.0F;
constexpr float DEFAULT_FOVY = 90.0F;
constexpr float DEFAULT_ASPECT_RATIO = 16.0F / 9.0F;
constexpr float DEFAULT_LOOK_SENSITIVITY = 0.1F;

constexpr float NEAR_PLANE = 0.01F;
constexpr float FAR_PLANE = 1000.0F;

class Camera
{
private:
    glm::vec3 position;
    float yaw = DEFAULT_YAW;
    float pitch = DEFAULT_PITCH;

    float fovy = DEFAULT_FOVY;
    float aspect_ratio = DEFAULT_ASPECT_RATIO;

    float look_sensitivity = DEFAULT_LOOK_SENSITIVITY;

    glm::vec3 up = WORLD_UP;
    glm::vec3 front = DEFAULT_FRONT;
    glm::vec3 right = DEFAULT_RIGHT;

    void update_direction_vectors() noexcept;

public:
    explicit Camera(glm::vec3 position, glm::vec2 angles = { DEFAULT_YAW, DEFAULT_PITCH }, float fovy = DEFAULT_FOVY,
                    float aspect_ratio = DEFAULT_ASPECT_RATIO) noexcept;

    explicit Camera(glm::vec3 position, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH, float fovy = DEFAULT_FOVY,
                    float aspect_ratio = DEFAULT_ASPECT_RATIO) noexcept;

    Camera() = delete;
    ~Camera();

    // disable copying
    Camera(const Camera&) = delete;
    auto operator=(const Camera&) -> Camera& = delete;

    // enable cheap moving
    Camera(Camera&&) noexcept = default;
    auto operator=(Camera&&) noexcept -> Camera& = default;

    void handle_look_input(float xoffset, float yoffset, bool constrain_pitch) noexcept;

    void set_position(glm::vec3 position) noexcept;
    void set_yaw(float yaw) noexcept;
    void set_pitch(float pitch) noexcept;
    void set_aspect_ratio(float aspect_ratio) noexcept;
    void set_look_sensitivity(float look_sensitivity) noexcept;

    [[nodiscard]] auto get_position() const noexcept -> glm::vec3;
    [[nodiscard]] auto get_yaw() const noexcept -> float;
    [[nodiscard]] auto get_pitch() const noexcept -> float;
    [[nodiscard]] auto get_front_vector() const noexcept -> glm::vec3;
    [[nodiscard]] auto get_up_vector() const noexcept -> glm::vec3;
    [[nodiscard]] auto get_right_vector() const noexcept -> glm::vec3;
    [[nodiscard]] auto get_aspect_ratio() const noexcept -> float;
    [[nodiscard]] auto get_look_sensitivity() const noexcept -> float;
    [[nodiscard]] auto get_view_matrix() const noexcept -> glm::mat4;
    [[nodiscard]] auto get_projection_matrix() const noexcept -> glm::mat4;
};

#endif // !VOXIDE_CAMERA_HEADER
