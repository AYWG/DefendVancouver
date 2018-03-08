//
// Created by Andy on 2018-02-21.
//

#include "attack.hpp"

Attack::STATUS Attack::tick(World *world, Enemy *enemy, float ms) {
    enemy->attack(ms);
    return SUCCESS;
}