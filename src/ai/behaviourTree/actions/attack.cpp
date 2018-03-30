//
// Created by Andy on 2018-02-21.
//

#include "attack.hpp"
#include "../../../entities/enemies/enemy.hpp"

Attack::STATUS Attack::tick(Enemy *enemy, float ms) {
    enemy->attack(ms);
    return SUCCESS;
}