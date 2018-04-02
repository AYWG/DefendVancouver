//
// Created by Andy on 2018-03-29.
//

#pragma once


#include "aI.hpp"
#include "../entities/enemies/enemy.hpp"

class BomberAI : public AI {
public:
    void doNextAction(Enemy *enemy, float ms) override;
};


