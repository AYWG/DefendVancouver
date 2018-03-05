//
// Created by Andy on 2018-02-25.
//

#pragma once


#include "aI.hpp"
#include "behaviourTree/behaviourTreeNode.hpp"

class World;
class Enemy;

class ShooterAI : public AI {
public:
    ShooterAI();

    void doNextAction(World *world, Enemy *enemy, float ms) override;

private:
    BehaviourTreeNode *m_root;

    void init();
};


