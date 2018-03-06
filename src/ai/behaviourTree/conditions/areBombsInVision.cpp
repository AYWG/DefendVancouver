//
// Created by Andy on 2018-03-05.
//

#include <vector>
#include "areBombsInVision.hpp"

AreBombsInVision::STATUS AreBombsInVision::tick(World *world, Enemy *enemy, float ms) {
    auto visionAngleFromNormal = 3.1415f / 6;
    auto enemyPosition = enemy->getPosition();

    std::vector<vec2> bombsInVision;
    for (auto bombPosition : world->getBombPositions()) {
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