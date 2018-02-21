//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../conditionNode.hpp"

class isPlayerInVision : public ConditionNode {
    STATUS tick() override;
};


