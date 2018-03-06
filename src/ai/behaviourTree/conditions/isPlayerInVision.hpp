//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../conditionNode.hpp"
#include "../../../world.hpp"

class IsObjectInVision : public ConditionNode {
public:
    explicit IsObjectInVision(vec2 objPosition);
    STATUS tick(World *world, Enemy *enemy, float ms) override;

private:
    vec2 m_objPosition;
};


