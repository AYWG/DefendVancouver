//
// Created by Andy on 2018-02-21.
//

#include <cmath>
#include "IsPlayerNearBomb.hpp"

IsPlayerNearBomb::STATUS IsPlayerNearBomb::tick(World *world, Enemy *enemy, float ms) {
    auto enemyPosition = enemy->getPosition();
    std::vector<vec2> bombsCloseToPlayer;

    for (auto bombInVision : enemy->getBombsInVision()) {
        // TODO: Improve this
        if (fabs(bombInVision.x - enemyPosition.x) < 100 &&
            fabs(bombInVision.y - enemyPosition.y) < 100) {
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