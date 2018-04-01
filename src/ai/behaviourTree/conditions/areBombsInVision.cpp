//
// Created by Andy on 2018-03-05.
//

#include <vector>
#include <cmath>
#include "areBombsInVision.hpp"
#include "../../../entities/enemies/enemy.hpp"


AreBombsInVision::STATUS AreBombsInVision::tick(Enemy *enemy, float ms) {
    auto visionAngleFromNormal = 3.1415f / 6;
    auto enemyPosition = enemy->getPosition();

    std::vector<vec2> bombsInVision;
    for (auto &bombPosition : enemy->getBombPositions()) {
        if (bombPosition.y > enemyPosition.y) {
            auto verticalDiff = bombPosition.y - enemyPosition.y;
            auto distanceToVisionBoundaryFromNormal = verticalDiff * tanf(visionAngleFromNormal);

            if (bombPosition.x < enemyPosition.x + distanceToVisionBoundaryFromNormal &&
                bombPosition.x > enemyPosition.x - distanceToVisionBoundaryFromNormal) {
                bombsInVision.emplace_back(bombPosition);
            }
        }
    }

    enemy->setBombsInVision(bombsInVision);

    if (!bombsInVision.empty()) {
        return SUCCESS;
    }
    return FAILURE;
}