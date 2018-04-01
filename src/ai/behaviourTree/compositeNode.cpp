//
// Created by Andy on 2018-02-20.
//

#include "compositeNode.hpp"

CompositeNode::CompositeNode(vector<unique_ptr<BehaviourTreeNode>> children)
        : m_children(std::move(children)){

}