//
// Created by Andy on 2018-02-25.
//

#include "shooterAI.hpp"
#include "behaviourTree/actions/moveIntoRange.hpp"

ShooterAI::ShooterAI() : m_root(nullptr) {
    init();
}

void ShooterAI::doNextAction(World *world, Enemy *enemy, float ms) {
    m_root->tick(world, enemy, ms);
}

void ShooterAI::init() {
    m_root = new MoveIntoRange();
}