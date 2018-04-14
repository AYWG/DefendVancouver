//
// Created by Andy on 2018-04-08.
//

#include "bomb.hpp"

Bomb::Bomb(World &world) : Entity(world), m_isHit(false) {}

void Bomb::onCollision(Entity &other) {

}

void Bomb::takeDamage() {
    m_isHit = true;
}

bool Bomb::isDamageable() const {
    return true;
}