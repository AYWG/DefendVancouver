//
// Created by Andy on 2018-02-20.
//

#pragma once


#include <vector>
#include "behaviourTreeNode.hpp"

using std::vector;

/**
 * A CompositeNode has one or more children. Each of its children are processed
 * one by one, in order (left to right).
 */

class CompositeNode : public BehaviourTreeNode {
public:
    CompositeNode(const vector<BehaviourTreeNode*>& children = vector<BehaviourTreeNode*>());

protected:
    vector<BehaviourTreeNode*> m_children;

};


