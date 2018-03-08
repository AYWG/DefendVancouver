//
// Created by Andy on 2018-03-03.
//

#include "bullet.hpp"

Bullet::Bullet() : m_scale({1.f, 1.f}), m_position({0.f, 0.f}), m_direction({0.f, 0.f}), m_speed(0.f) {}

vec2 Bullet::getPosition() const {
    return m_position;
}

void Bullet::setPosition(vec2 position) {
    m_position = position;
}

void Bullet::setDirection(vec2 direction) {
    m_direction = direction;
}

void Bullet::setSpeed(float speed) {
    m_speed = speed;
}