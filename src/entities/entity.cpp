//
// Created by Andy on 2018-03-07.
//

#include "entity.hpp"

Entity::Entity() : m_position({0.f, 0.f}), m_scale({1.f, 1.f}) {}

vec2 Entity::getPosition() const {
    return m_position;
}

void Entity::setPosition(vec2 position) {
    m_position = position;
}