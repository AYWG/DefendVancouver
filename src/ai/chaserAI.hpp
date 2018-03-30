//
// Created by Andy on 2018-02-25.
//

#pragma once


#include "aI.hpp"
#include "behaviourTree/behaviourTreeNode.hpp"

class Enemy;

class ChaserAI : public AI {
public:
    ChaserAI();

    void doNextAction(Enemy *enemy, float ms) override;

private:
    BehaviourTreeNode *m_root;

    void init();
};


