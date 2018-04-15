//
// Created by Andy on 2018-03-07.
//

#include "entity.hpp"
#include "../world.hpp"

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

vec2 Entity::getPlayerPosition() const {
    return m_world->getPlayerPosition();
}


bool Entity::isCollidingWith(Entity &other) const {
    Region myBox = getBoundingBox();
    Region otherBox = other.getBoundingBox();

    return (myBox.origin.x < otherBox.origin.x + otherBox.size.x &&
            myBox.origin.x + myBox.size.x > otherBox.origin.x &&
            myBox.origin.y < otherBox.origin.y + otherBox.size.y &&
            myBox.origin.y + myBox.size.y > otherBox.origin.y);
}

void Entity::die() {
    m_isDead = true;
}

bool Entity::isDead() const {
    return m_isDead;
}