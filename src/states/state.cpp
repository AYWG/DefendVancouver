//
// Created by Tanwin Nun on 2018-04-11.
//

#include "state.hpp"

bool State::initGraphics() {
    return false;
}

State::~State() {

}

State::State(World &world) : Entity(world) {

}

void State::setPosition(vec2 pos) {
    m_position.x = pos.x;
    m_position.y = pos.y;
}

bool State::init() {
    m_scale.x = 1.0f;
    m_scale.y = 1.0f;

    return true;
}

void State::onCollision(Entity &other) {

}

void State::takeDamage() {

}

bool State::isDamageable() const {
    return false;
}

Entity::FACTION State::getFaction() const {
    return ALIEN;
}
