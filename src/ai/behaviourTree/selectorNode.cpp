//
// Created by Andy on 2018-02-20.
//

#include "selectorNode.hpp"

SelectorNode::SelectorNode(vector<unique_ptr<BehaviourTreeNode>> children) : CompositeNode(std::move(children)) {}

SelectorNode::STATUS SelectorNode::tick(Enemy *enemy, float ms) {
    for (auto &child : m_children) {
        STATUS childStatus = child->tick(enemy, ms);
        if (childStatus == RUNNING) {
            return RUNNING;
        } else if (childStatus == SUCCESS) {
            return SUCCESS;
        }
    }
    return FAILURE;
}