//
// Created by Andy on 2018-03-03.
//

#include "bullet.hpp"

Bullet::Bullet(World &world) : Entity(world), m_velocity({0.f, 0.f}) {}

void Bullet::setVelocity(vec2 velocity) {
    m_velocity = velocity;
}