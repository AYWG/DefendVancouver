//
// Created by Andy on 2018-02-21.
//

#include "moveToLocation.hpp"

MoveToLocation::MoveToLocation(vec2& location) : m_location(location) {

}

// TODO: make this more generic
MoveToLocation::STATUS MoveToLocation::tick(World *world, Enemy *enemy, float ms) {
    // Check if we've arrived at the desired location
    if (enemy->getPosition().y >= m_location.y) {
        return SUCCESS;
    }

    // Otherwise we're not done yet
    float speed = 200.f;
    float step = speed * (ms / 1000);
    enemy->setPosition({enemy->getPosition().x, enemy->getPosition().y + step});

    return RUNNING;
}