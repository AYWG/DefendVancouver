//
// Created by Andy on 2018-02-26.
//

#include "repeatUntilFailure.hpp"

RepeatUntilFailure::RepeatUntilFailure(BehaviourTreeNode *child) : DecoratorNode(child) {

}

RepeatUntilFailure::STATUS RepeatUntilFailure::tick(World *world, Enemy *enemy, float ms) {
    STATUS childStatus = m_child->tick(world, enemy, ms);
    if (childStatus == FAILURE) {
        return SUCCESS;
    }

    return RUNNING;
}