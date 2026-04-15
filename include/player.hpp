#pragma once

#include <entity.hpp>
#include <optional>

#include "camera.hpp"

class Player: Entity
{
private:
    std::optional<Camera> camera;
    bool local = false;

public:
    explicit Player(bool local = false);

    [[nodiscard]] auto is_local() const noexcept -> bool;
    [[nodiscard]] auto get_camera() noexcept -> Camera*;

    void tick(double delta_time) override;
};
