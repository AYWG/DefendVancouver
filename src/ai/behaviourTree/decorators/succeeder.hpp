//
// Created by Andy on 2018-03-05.
//

#pragma once


#include "decoratorNode.hpp"

class Succeeder : public DecoratorNode {
public:
    explicit Succeeder(unique_ptr<BehaviourTreeNode> child);

    STATUS tick(Enemy *enemy, float ms) override;
};


