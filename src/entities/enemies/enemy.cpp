//
// Created by Andy on 2018-02-19.
//

#include "enemy.hpp"

float Enemy::getAngleToTarget() const {
    return m_angleToTarget;
}

void Enemy::setAngleToTarget(float angle) {
    m_angleToTarget = angle;
}

std::vector<vec2> Enemy::getBombsInVision() const {
    return m_bombsInVision;
}

void Enemy::setBombsInVision(std::vector<vec2> &bombPositions) {
    m_bombsInVision = bombPositions;
}