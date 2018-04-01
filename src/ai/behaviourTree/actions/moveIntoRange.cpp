//
// Created by Andy on 2018-02-21.
//

#include "moveIntoRange.hpp"
#include "../../../entities/enemies/enemy.hpp"


// TODO: make this more generic
MoveIntoRange::STATUS MoveIntoRange::tick(Enemy *enemy, float ms) {
    // Check if in range
    if (enemy->getCityPosition().y - enemy->getPosition().y <= 500) {
        return SUCCESS;
    }

    // Otherwise we're not done yet
    float speed = 200.f;
    float step = speed * (ms / 1000);
    enemy->setPosition({enemy->getPosition().x, enemy->getPosition().y + step});

    return RUNNING;
}