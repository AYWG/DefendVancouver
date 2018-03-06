//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../conditionNode.hpp"
#include "../../../world.hpp"

class IsPlayerInVision : public ConditionNode {
public:
    STATUS tick(World *world, Enemy *enemy, float ms) override;
};


