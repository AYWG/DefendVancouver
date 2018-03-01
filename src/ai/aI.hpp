//
// Created by Andy on 2018-02-25.
//

#pragma once


class World;
class Enemy;
class AI {
public:
    virtual void doNextAction(World *world, Enemy *enemy, float ms) = 0;
};


