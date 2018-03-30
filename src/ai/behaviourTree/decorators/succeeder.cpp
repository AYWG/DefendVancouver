//
// Created by Andy on 2018-03-05.
//

#include "succeeder.hpp"

Succeeder::Succeeder(unique_ptr<BehaviourTreeNode> child) : DecoratorNode(std::move(child)) {}

Succeeder::STATUS Succeeder::tick(Enemy *enemy, float ms) {
    STATUS childStatus = m_child->tick(enemy, ms);
    if (childStatus == RUNNING) {
        return RUNNING;
    }

    return SUCCESS;
}