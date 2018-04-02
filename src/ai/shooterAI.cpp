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
#include "behaviourTree/actions/attack.hpp"

ShooterAI::ShooterAI() : m_root(nullptr) {
    init();
}

void ShooterAI::doNextAction(Enemy *enemy, float ms) {
    m_root->tick(enemy, ms);
}

void ShooterAI::init() {
    vector<std::unique_ptr<BehaviourTreeNode>> findTargetSequenceChildren;
    findTargetSequenceChildren.emplace_back(std::unique_ptr<IsPlayerInVision>(new IsPlayerInVision));
    findTargetSequenceChildren.emplace_back(std::unique_ptr<AreBombsInVision>(new AreBombsInVision));
    findTargetSequenceChildren.emplace_back(std::unique_ptr<IsPlayerNearBomb>(new IsPlayerNearBomb));

    auto findTargetSequenceNode = std::unique_ptr<SequenceNode>(new SequenceNode(std::move(findTargetSequenceChildren)));
    auto succeederNode = std::unique_ptr<Succeeder>(new Succeeder(std::move(findTargetSequenceNode)));

    vector<std::unique_ptr<BehaviourTreeNode>> attackingSequenceChildren;
    attackingSequenceChildren.emplace_back(std::move(succeederNode));
    attackingSequenceChildren.emplace_back(std::unique_ptr<RotateToTarget>(new RotateToTarget));
    attackingSequenceChildren.emplace_back(std::unique_ptr<Attack>(new Attack));

    auto attackingSequenceNode = std::unique_ptr<SequenceNode>(new SequenceNode(std::move(attackingSequenceChildren)));
    auto repeatUntilFailureNode = std::unique_ptr<RepeatUntilFailure>(new RepeatUntilFailure(std::move(attackingSequenceNode)));

    vector<std::unique_ptr<BehaviourTreeNode>> rootSequenceChildren;
    rootSequenceChildren.emplace_back(std::unique_ptr<MoveIntoRange>(new MoveIntoRange));
    rootSequenceChildren.emplace_back(std::move(repeatUntilFailureNode));

    auto rootSequenceNode = std::unique_ptr<SequenceNode>(new SequenceNode(std::move(rootSequenceChildren)));

    m_root = std::move(rootSequenceNode);
}