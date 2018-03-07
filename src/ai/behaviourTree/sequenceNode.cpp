//
// Created by Andy on 2018-02-20.
//

#include "sequenceNode.hpp"

SequenceNode::SequenceNode(const vector<BehaviourTreeNode *> &children) : CompositeNode(children) {

}

SequenceNode::STATUS SequenceNode::tick(World *world, Enemy *enemy, float ms) {
    for (auto &child : m_children) {
        STATUS childStatus = child->tick(world, enemy, ms);
        if (childStatus == RUNNING) {
            return RUNNING;
        } else if (childStatus == FAILURE) {
            return FAILURE;
        }
    }
    return SUCCESS;
}
