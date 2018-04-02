//
// Created by Andy on 2018-02-21.
//

#include <cmath>
#include "IsPlayerNearBomb.hpp"
#include "../../../entities/enemies/enemy.hpp"


IsPlayerNearBomb::STATUS IsPlayerNearBomb::tick(Enemy *enemy, float ms) {
    auto enemyPosition = enemy->getPosition();
    auto playerPosition = enemy->getPlayerPosition();
    std::vector<vec2> bombsCloseToPlayer;

    for (auto &bombInVision : enemy->getBombsInVision()) {
        // TODO: Improve this
        if (std::abs(bombInVision.x - playerPosition.x) < 100 &&
            std::abs(bombInVision.y - playerPosition.y) < 100) {
            bombsCloseToPlayer.emplace_back(bombInVision);
            break;
        }
    }

    if (!bombsCloseToPlayer.empty()) {
        auto yDiff = bombsCloseToPlayer.front().y - enemyPosition.y;
        auto xDiff = bombsCloseToPlayer.front().x - enemyPosition.x;
        enemy->setAngleToTarget(-1.f * atanf(xDiff / yDiff));
        return SUCCESS;
    }

    return FAILURE;
}