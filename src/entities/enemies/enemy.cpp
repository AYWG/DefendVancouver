//
// Created by Andy on 2018-02-19.
//

#include "enemy.hpp"
#include "../../world.hpp"


Enemy::Enemy(World &world, AI &ai) : Entity(world), m_ai(std::unique_ptr<AI>(&ai)) {}

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

vec2 Enemy::getPlayerPosition() const {
    return m_world->getPlayerPosition();
}

std::vector<vec2> Enemy::getBombPositions() const {
    return m_world->getBombPositions();
}

vec2 Enemy::getCityPosition() const {
    return m_world->getCityPosition();
}