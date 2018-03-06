//
// Created by Andy on 2018-02-21.
//

#include "rotateToTarget.hpp"

RotateToTarget::STATUS RotateToTarget::tick(World *world, Enemy *enemy, float ms) {
    auto targetAngle = enemy->getAngleToTarget();
    auto enemyRotation = enemy->getRotation();

    if (targetAngle > enemyRotation) enemy->setRotation(std::min(targetAngle, enemyRotation + 0.01f));
    if (targetAngle < enemyRotation) enemy->setRotation(std::max(targetAngle, enemyRotation - 0.01f));

    return SUCCESS;
}
