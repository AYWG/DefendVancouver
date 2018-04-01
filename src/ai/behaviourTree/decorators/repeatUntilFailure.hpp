//
// Created by Andy on 2018-02-26.
//

#pragma once


#include "decoratorNode.hpp"

class RepeatUntilFailure : public DecoratorNode {
public:
    explicit RepeatUntilFailure(unique_ptr<BehaviourTreeNode> child);

    STATUS tick(Enemy *enemy, float ms) override;
};


