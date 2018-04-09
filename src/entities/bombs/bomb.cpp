//
// Created by Andy on 2018-04-08.
//

#include "bomb.hpp"

Bomb::Bomb(World &world) : Entity(world) {}

void Bomb::onCollision(Entity &other) {

}

void Bomb::takeDamage() {

}

bool Bomb::isDamageable() const {
    return true;
}