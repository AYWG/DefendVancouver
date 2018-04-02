//
// Created by Andy on 2018-02-25.
//

#pragma once

class Enemy;

class AI {
public:
    virtual void doNextAction(Enemy *enemy, float ms) = 0;
};


