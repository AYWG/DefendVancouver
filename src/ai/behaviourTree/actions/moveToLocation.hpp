//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../actionNode.hpp"
#include "../../../common.hpp"
#include "../../../enemies/enemy.hpp"

class MoveToLocation : public ActionNode {
public:
    explicit MoveToLocation(vec2& location);
    STATUS tick(World *world, Enemy *enemy, float ms) override;

private:
    vec2 m_location;
};



