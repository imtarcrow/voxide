#include "player.hpp"

#include <SDL3/SDL_keyboard.h>

Player::Player(bool local)
    : local(local)
{
    if (local) {
        this->camera.emplace(this->position, this->yaw, this->pitch);
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

void Player::tick(float delta_time)
{
    if (local) {
        this->camera->set_position(this->position + glm::vec3 { 0.0F, 1.8F, 0.0F });
        const bool* keys = SDL_GetKeyboardState(nullptr);

        const float sensitivity = 20.0F;

        if (keys[SDL_SCANCODE_LEFT]) {
            this->camera->handle_look_input(-1000.0F * delta_time, 0.0F * delta_time, true);
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            this->camera->handle_look_input(1000.0F * delta_time, 0.0F * delta_time, true);
        }

        if (keys[SDL_SCANCODE_UP]) {
            this->camera->handle_look_input(0.0F * delta_time, 1000.0F * delta_time, true);
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            this->camera->handle_look_input(0.0F * delta_time, -1000.0F * delta_time, true);
        }

        if (keys[SDL_SCANCODE_W]) {
            this->set_position(this->get_position() + this->camera->get_front_vector() * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_S]) {
            this->set_position(this->get_position() - this->camera->get_front_vector() * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_D]) {
            this->set_position(this->get_position() + this->camera->get_right_vector() * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_A]) {
            this->set_position(this->get_position() - this->camera->get_right_vector() * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_SPACE]) {
            this->set_position(this->get_position() + glm::vec3(0.0, 1.0, 0.0) * sensitivity * delta_time);
        }

        if (keys[SDL_SCANCODE_LSHIFT]) {
            this->set_position(this->get_position() - glm::vec3(0.0, 1.0, 0.0) * sensitivity * delta_time);
        }
    }
}
