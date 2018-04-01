//
// Created by Andy on 2018-02-25.
//

#pragma once


#include <memory>
#include "../behaviourTreeNode.hpp"

using std::unique_ptr;

/**
 * A DecoratorNode has a single child
 */

class DecoratorNode : public BehaviourTreeNode {
public:
    explicit DecoratorNode(unique_ptr<BehaviourTreeNode> child);

protected:
    unique_ptr<BehaviourTreeNode> m_child;
};


