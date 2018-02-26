//
// Created by Andy on 2018-02-25.
//

#include "shooterAI.hpp"

ShooterAI::ShooterAI() : m_root(nullptr) {}

void ShooterAI::doNextAction(World *world, Enemy *enemy, float ms) {
    m_root->tick(world, enemy, ms);
}