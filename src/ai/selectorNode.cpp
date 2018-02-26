//
// Created by Andy on 2018-02-20.
//

#include "selectorNode.hpp"

SelectorNode::SelectorNode(const vector<BehaviourTreeNode *> &children) : CompositeNode(children) {

}

SelectorNode::STATUS SelectorNode::tick(World *world, Enemy *enemy, float ms) {
    for (auto &child : m_children) {
        STATUS childStatus = child->tick(world, enemy, ms);
        if (childStatus == RUNNING) {
            return RUNNING;
        } else if (childStatus == SUCCESS) {
            return SUCCESS;
        }
    }
    return FAILURE;
}