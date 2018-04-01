//
// Created by Andy on 2018-03-07.
//

#include "entity.hpp"

Entity::Entity(World &world) : m_world(&world), m_position({0.f, 0.f}), m_rotation(0.f), m_scale({1.f, 1.f}),
                               m_isDead(false) {}

vec2 Entity::getPosition() const {
    return m_position;
}

void Entity::setPosition(vec2 position) {
    m_position = position;
}

float Entity::getRotation() const {
    return m_rotation;
}

void Entity::setRotation(float radians) {
    m_rotation = radians;
}

bool Entity::isCollidingWith(Entity &other) const {
    auto dx = m_position.x - other.getPosition().x;
    auto dy = m_position.y - other.getPosition().y;
    auto d_sq = dx * dx + dy * dy;
    auto myDiameter = std::max(getBoundingBox().size.x, getBoundingBox().size.y);
    auto otherDiameter = std::max(other.getBoundingBox().size.x, other.getBoundingBox().size.y);
    auto combinedRadii = (myDiameter + otherDiameter) / 2;
    return d_sq < combinedRadii * combinedRadii;
}

void Entity::die() {
    m_isDead = true;
}

bool Entity::isDead() const {
    return m_isDead;
}