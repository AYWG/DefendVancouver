//
// Created by Andy on 2018-02-25.
//

#pragma once


#include <memory>
#include "aI.hpp"
#include "behaviourTree/behaviourTreeNode.hpp"

class Enemy;

class ShooterAI : public AI {
public:
    ShooterAI();

    void doNextAction(Enemy *enemy, float ms) override;

private:
    std::unique_ptr<BehaviourTreeNode> m_root;

    /**
     * Builds the behaviour tree.
     */
    void init();
};


