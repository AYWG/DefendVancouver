//
// Created by Andy on 2018-03-29.
//

#include "bomberAI.hpp"

void BomberAI::doNextAction(Enemy *enemy, float ms) {
    const float SPEED = 250.f;
    float step = SPEED * (ms / 1000);
    enemy->setPosition({enemy->getPosition().x + step, enemy->getPosition().y});

    if (enemy->isInView()) {
        enemy->attack(ms);
    }
}

