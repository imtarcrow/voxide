#pragma once

#include <atomic>
#include <glm/glm.hpp>

class Entity
{
protected:
    static inline std::atomic<std::uint64_t> id_counter { 0 };
    std::uint64_t entity_id = id_counter.fetch_add(1, std::memory_order_relaxed);

    glm::vec3 position = { 0.0F, 0.0F, 0.0F };
    float yaw = 0.0, pitch = 0.0;

public:
    Entity() = default;
    virtual ~Entity() = default;

    Entity(const Entity&) = default;
    Entity(Entity&&) = delete;
    auto operator=(const Entity&) -> Entity& = default;
    auto operator=(Entity&&) -> Entity& = delete;

    virtual void tick(double delta_time) = 0;

    [[nodiscard]] auto get_id() const noexcept -> std::uint64_t;

    [[nodiscard]] auto get_position() const noexcept -> glm::vec3;
    void set_position(glm::vec3 position) noexcept;

    [[nodiscard]] auto get_yaw() const noexcept -> float;
    void set_yaw(float yaw) noexcept;

    [[nodiscard]] auto get_pitch() const noexcept -> float;
    void set_pitch(float pitch) noexcept;
};
