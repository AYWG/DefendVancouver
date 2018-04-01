//
// Created by Andy on 2018-02-20.
//

#pragma once


class Enemy;

class BehaviourTreeNode {
public:
    typedef enum {
        SUCCESS, FAILURE, RUNNING
    } STATUS;

    virtual STATUS tick(Enemy *enemy, float ms) = 0;
};


