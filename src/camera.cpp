//
// Created by Andy on 2018-02-08.
//

#include "camera.hpp"

void Camera::move(vec2 newPos) {
    m_pos = newPos;
}

vec2 Camera::getOrigin() {
    return {m_pos.x - (m_size.x / 2.f), m_pos.y - (m_size.y / 2)};
}
