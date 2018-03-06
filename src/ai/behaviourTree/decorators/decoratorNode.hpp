//
// Created by Andy on 2018-02-25.
//

#pragma once


#include "../behaviourTreeNode.hpp"

/**
 * A DecoratorNode has a single child
 */

class DecoratorNode : public BehaviourTreeNode {
public:
    explicit DecoratorNode(BehaviourTreeNode* child);

protected:
    BehaviourTreeNode* m_child;
};


