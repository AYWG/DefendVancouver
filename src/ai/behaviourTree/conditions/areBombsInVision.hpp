//
// Created by Andy on 2018-03-05.
//

#pragma once


#include "../conditionNode.hpp"
#include "../../../entities/enemies/enemy.hpp"

class AreBombsInVision : public ConditionNode {
public:
    STATUS tick(Enemy *enemy, float ms) override;
};


