#include "player.hpp"

Player::Player(bool local) : local(local) {
    if(local) {
        camera = Camera {this->position, this->yaw, this->pitch };
    }
}

auto Player::is_local() const noexcept -> bool
{
    return this->local;
}

auto Player::get_camera() noexcept -> Camera*
{
    if (!this->local)
        return nullptr;
    return &this->camera.value();
}

void Player::tick(double delta_time)
{

}
