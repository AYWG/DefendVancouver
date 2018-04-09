//
// Created by Andy on 2018-04-08.
//

#include "powerUp.hpp"
#include "../../world.hpp"

PowerUp::PowerUp(World &world) : Entity(world) {}

void PowerUp::update(float ms) {
    const float SPEED = 100.f;
    float step = SPEED * (ms / 1000);
    m_position.y += step;

    if (m_position.y > m_world->getSize().y) {
        die();
    }
}

void PowerUp::takeDamage() {
    // PowerUps cannot take damage
}

bool PowerUp::isDamageable() const {
    return false;
}

PowerUp::FACTION PowerUp::getFaction() const {
    return FACTION::HUMAN;
}