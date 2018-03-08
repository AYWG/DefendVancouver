//
// Created by Andy on 2018-03-05.
//

#pragma once


#include "decoratorNode.hpp"

class Succeeder : public DecoratorNode {
public:
    explicit Succeeder(BehaviourTreeNode* child);

    STATUS tick(World *world, Enemy *enemy, float ms) override;
};


