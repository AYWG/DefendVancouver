//
// Created by Andy on 2018-02-25.
//

#include "behaviourTree/actions/moveIntoRange.hpp"
#include "chaserAI.hpp"

ChaserAI::ChaserAI() : m_root(nullptr) {
    init();
}

void ChaserAI::doNextAction(Enemy *enemy, float ms) {
    m_root->tick(enemy, ms);
}

void ChaserAI::init() {
    //m_root = new MoveIntoRange();
}