//
// Created by Andy on 2018-02-21.
//

#include <cmath>
#include "isPlayerInVision.hpp"
#include "../../../entities/enemies/enemy.hpp"


IsPlayerInVision::STATUS IsPlayerInVision::tick(Enemy *enemy, float ms) {
    auto visionAngleFromNormal = 3.1415f / 6;
    auto playerPosition = enemy->getPlayerPosition();
    auto enemyPosition = enemy->getPosition();

    if (playerPosition.y > enemyPosition.y) {
        auto verticalDiff = playerPosition.y - enemyPosition.y;
        auto distanceToVisionBoundaryFromNormal = verticalDiff * tanf(visionAngleFromNormal);

        if (playerPosition.x < enemyPosition.x + distanceToVisionBoundaryFromNormal &&
            playerPosition.x > enemyPosition.x - distanceToVisionBoundaryFromNormal) {
            auto yDiff = playerPosition.y - enemyPosition.y;
            auto xDiff = playerPosition.x - enemyPosition.x;
            enemy->setAngleToTarget(-1.f * atanf(xDiff / yDiff));
            return SUCCESS;
        }
    }
    enemy->setAngleToTarget(0.f);
    return FAILURE;
}
