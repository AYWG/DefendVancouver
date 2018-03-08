//
// Created by Andy on 2018-03-05.
//

#include "succeeder.hpp"

Succeeder::Succeeder(BehaviourTreeNode *child) : DecoratorNode(child) {

}

Succeeder::STATUS Succeeder::tick(World *world, Enemy *enemy, float ms) {
    STATUS childStatus = m_child->tick(world, enemy, ms);
    if (childStatus == RUNNING) {
        return RUNNING;
    }

    return SUCCESS;
}