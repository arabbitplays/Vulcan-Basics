//
// Created by oster on 14.09.2024.
//

#ifndef BASICS_CAMERA_HPP
#define BASICS_CAMERA_HPP


#include <glm/glm.hpp>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"

class Camera {
public:
    glm::vec3 velocity;
    glm::vec3 position;
    // vertical rotation
    float pitch { 0.f };
    // horizontal rotation
    float yaw { 0.f };

    glm::mat4 getViewMatrix();
    glm::mat4 getRotationMatrix();

    void processSDLEvent(SDL_Event& e);

    void update();
};


#endif //BASICS_CAMERA_HPP
