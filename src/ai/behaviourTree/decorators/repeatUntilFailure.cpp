//
// Created by Andy on 2018-02-26.
//

#include "repeatUntilFailure.hpp"

RepeatUntilFailure::RepeatUntilFailure(unique_ptr<BehaviourTreeNode> child) : DecoratorNode(std::move(child)) {

}

RepeatUntilFailure::STATUS RepeatUntilFailure::tick(Enemy *enemy, float ms) {
    STATUS childStatus = m_child->tick(enemy, ms);
    if (childStatus == FAILURE) {
        return SUCCESS;
    }

    return RUNNING;
}