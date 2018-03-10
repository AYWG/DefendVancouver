//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../actionNode.hpp"
#include "../../../entities/enemies/enemy.hpp"

/**
 * An attack action.
 */

class Attack : public ActionNode {
    STATUS tick(World *world, Enemy *enemy, float ms) override;
};


