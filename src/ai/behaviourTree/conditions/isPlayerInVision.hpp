//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../conditionNode.hpp"


class IsPlayerInVision : public ConditionNode {
public:
    STATUS tick(Enemy *enemy, float ms) override;
};


