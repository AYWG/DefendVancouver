//
// Created by Andy on 2018-02-21.
//

#pragma once

#include "../actionNode.hpp"
#include "../../../entities/enemies/enemy.hpp"

class RotateToTarget : public ActionNode {
    STATUS tick(Enemy *enemy, float ms) override;
};


