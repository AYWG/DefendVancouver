//
// Created by Andy on 2018-02-25.
//

#include "shooterAI.hpp"
#include "behaviourTree/actions/moveIntoRange.hpp"
#include "behaviourTree/actions/rotateToTarget.hpp"
#include "behaviourTree/conditions/isPlayerInVision.hpp"
#include "behaviourTree/conditions/areBombsInVision.hpp"
#include "behaviourTree/conditions/IsPlayerNearBomb.hpp"
#include "behaviourTree/sequenceNode.hpp"
#include "behaviourTree/decorators/repeatUntilFailure.hpp"
#include "behaviourTree/decorators/succeeder.hpp"

ShooterAI::ShooterAI() : m_root(nullptr) {
    init();
}

void ShooterAI::doNextAction(World *world, Enemy *enemy, float ms) {
    m_root->tick(world, enemy, ms);
}

void ShooterAI::init() {
    auto moveIntoRangeNode = new MoveIntoRange();
    auto rotateToTargetNode = new RotateToTarget();
    auto isPlayerInVisionNode = new IsPlayerInVision();
    auto areBombsInVisionNode = new AreBombsInVision();
    auto isPlayerNearBombNode = new IsPlayerNearBomb();

    vector<BehaviourTreeNode *> findTargetSequenceChildren = {isPlayerInVisionNode, areBombsInVisionNode, isPlayerNearBombNode};
    auto findTargetSequenceNode = new SequenceNode(findTargetSequenceChildren);
    auto succeederNode = new Succeeder(findTargetSequenceNode);

    vector<BehaviourTreeNode *> attackingSequenceChildren = {succeederNode, rotateToTargetNode};
    auto attackingSequenceNode = new SequenceNode(attackingSequenceChildren);

    auto repeatUntilFailureNode = new RepeatUntilFailure(attackingSequenceNode);

    vector<BehaviourTreeNode *> rootSequenceChildren = {moveIntoRangeNode, repeatUntilFailureNode};
    auto rootSequenceNode = new SequenceNode(rootSequenceChildren);

    m_root = rootSequenceNode;
}