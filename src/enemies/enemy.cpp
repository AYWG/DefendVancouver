//
// Created by Andy on 2018-02-19.
//

#include "enemy.hpp"

vec2 Enemy::getPosition() const {
    return m_position;
}

void Enemy::setPosition(vec2 position) {
    m_position = position;
}