//
// Created by Andy on 2018-02-26.
//

#pragma once


#include "decoratorNode.hpp"

class RepeatUntilFailure : public DecoratorNode {
public:
    explicit RepeatUntilFailure(BehaviourTreeNode* child);

    STATUS tick(World *world, Enemy *enemy, float ms) override;
};


