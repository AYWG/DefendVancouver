//
// Created by Andy on 2018-02-26.
//

#pragma once


#include "decoratorNode.hpp"

class RepeatUntilSuccess : public DecoratorNode {
public:
    explicit RepeatUntilSuccess(BehaviourTreeNode* child);

    STATUS tick(World *world, Enemy *enemy, float ms) override;
};


