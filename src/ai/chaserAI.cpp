//
// Created by Andy on 2018-02-25.
//

#include "behaviourTree/actions/moveToLocation.hpp"
#include "chaserAI.hpp"

ChaserAI::ChaserAI() : m_root(nullptr) {
    init();
}

void ChaserAI::doNextAction(World *world, Enemy *enemy, float ms) {
    m_root->tick(world, enemy, ms);
}

void ChaserAI::init() {
    //m_root = new MoveToLocation();
}