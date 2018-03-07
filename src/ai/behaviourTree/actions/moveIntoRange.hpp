//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../actionNode.hpp"
#include "../../../common.hpp"
#include "../../../world.hpp"
#include "../../../enemies/enemy.hpp"

class MoveIntoRange : public ActionNode {
public:
    STATUS tick(World *world, Enemy *enemy, float ms) override;

};



