//
// Created by Andy on 2018-04-08.
//

#include "bomb.hpp"

Bomb::Bomb(World &world) : Entity(world), m_isHit(false) {}

void Bomb::takeDamage() {
    explode();
}

bool Bomb::isDamageable() const {
    return true;
}

void Bomb::explode() {
    m_isHit = true;
}

bool Bomb::isInvulnerable() const {
    return m_invulnerabilityCountdown >= 0;
}