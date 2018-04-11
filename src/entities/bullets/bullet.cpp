//
// Created by Andy on 2018-03-03.
//

#include "bullet.hpp"

Bullet::Bullet(World &world) : Entity(world), m_velocity({0.f, 0.f}) {}

void Bullet::setVelocity(vec2 velocity) {
    m_velocity = velocity;
}

void Bullet::onCollision(Entity &other) {
    if (!m_isDead && other.isDamageable() && other.getFaction() != getFaction()) {
        other.takeDamage();
        // The bullet has done damage; it is now dead
        die();
    }
}

void Bullet::takeDamage() {
    // Bullets cannot take damage
}

bool Bullet::isDamageable() const {
    return false;
}