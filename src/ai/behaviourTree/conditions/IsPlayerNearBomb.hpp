//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../conditionNode.hpp"
#include "../../../enemies/enemy.hpp"

class IsPlayerNearBomb : public ConditionNode {
public:
    STATUS tick(World *world, Enemy *enemy, float ms) override;
};


