//
// Created by Andy on 2018-02-20.
//

#include "sequenceNode.hpp"

SequenceNode::STATUS SequenceNode::tick() {
    for (auto& child : m_children) {
        STATUS childStatus = child->tick();
        if (childStatus == RUNNING) {
            return RUNNING;
        } else if (childStatus == FAILURE) {
            return FAILURE;
        }
    }
    return SUCCESS;
}
