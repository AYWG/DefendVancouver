//
// Created by Andy on 2018-02-20.
//

#include "sequenceNode.hpp"

SequenceNode::SequenceNode(vector<unique_ptr<BehaviourTreeNode>> children) : CompositeNode(std::move(children)) {

}

SequenceNode::STATUS SequenceNode::tick(Enemy *enemy, float ms) {
    for (auto &child : m_children) {
        STATUS childStatus = child->tick(enemy, ms);
        if (childStatus == RUNNING) {
            return RUNNING;
        } else if (childStatus == FAILURE) {
            return FAILURE;
        }
    }
    return SUCCESS;
}
