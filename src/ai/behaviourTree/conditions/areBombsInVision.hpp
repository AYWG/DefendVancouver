//
// Created by Andy on 2018-03-05.
//

#pragma once


#include "../conditionNode.hpp"
#include "../../../world.hpp"

class AreBombsInVision : public ConditionNode {
public:
    STATUS tick(World *world, Enemy *enemy, float ms) override;
};


