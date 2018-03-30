//
// Created by Andy on 2018-02-25.
//

#include "decoratorNode.hpp"

DecoratorNode::DecoratorNode(unique_ptr<BehaviourTreeNode> child) : m_child(std::move(child)) {

}
