//
// Created by Andy on 2018-02-20.
//

#pragma once


#include <vector>
#include <memory>
#include "behaviourTreeNode.hpp"

using std::vector;
using std::unique_ptr;

/**
 * A CompositeNode has one or more children. Each of its children are processed
 * one by one, in order (left to right).
 */

class CompositeNode : public BehaviourTreeNode {
public:
    explicit CompositeNode(vector<unique_ptr<BehaviourTreeNode>> children);

protected:
    vector<unique_ptr<BehaviourTreeNode>> m_children;

};


