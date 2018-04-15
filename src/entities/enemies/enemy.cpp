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

bool Enemy::isInView() const {
    return m_world->isEntityInView(*this);
}

void Enemy::onCollision(Entity &other) {
    if (other.isDamageable() && typeid(other) == typeid(Player)) {
        other.takeDamage();
    }
}

void Enemy::takeDamage() {
    // TODO: only add points if damage taken causes enemy health to go below 0
    m_world->addPoints(getPoints());
    m_world->decrementRemainingEnemies();
    die();
}

bool Enemy::isDamageable() const {
    return true;
}

Enemy::FACTION Enemy::getFaction() const {
    return FACTION::ALIEN;
}