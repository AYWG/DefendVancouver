//
// Created by Andy on 2018-02-19.
//

#include "enemy.hpp"

Enemy::Enemy()
        : m_position({0.f, 0.f}),
          m_scale({1.f, 1.f}),
          m_rotation(0.f) {

}

vec2 Enemy::getPosition() const {
    return m_position;
}

void Enemy::setPosition(vec2 position) {
    m_position = position;
}