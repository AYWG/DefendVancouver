//
// Created by Andy on 2018-02-20.
//

#include "selectorNode.hpp"

SelectorNode::STATUS SelectorNode::tick() {
    for (auto& child : m_children) {
        STATUS childStatus = child->tick();
        if (childStatus == RUNNING) {
            return RUNNING;
        } else if (childStatus == SUCCESS) {
            return SUCCESS;
        }
    }
    return FAILURE;
}