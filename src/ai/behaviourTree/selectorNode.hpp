//
// Created by Andy on 2018-02-20.
//

#pragma once


#include "compositeNode.hpp"

/**
 * A SelectorNode returns SUCCESS upon the first child succeeding; RUNNING if any child is being processed, and
 * FAILURE if all children return FAILURE.
 */

class SelectorNode : public CompositeNode {
public:
    explicit SelectorNode(vector<unique_ptr<BehaviourTreeNode>> children);
    SelectorNode::STATUS tick(Enemy *enemy, float ms) override;
};


