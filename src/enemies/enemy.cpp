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

float Enemy::getRotation() const {
    return m_rotation;
}

void Enemy::setRotation(float rotation) {
    m_rotation = rotation;
}

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