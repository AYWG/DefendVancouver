//
// Created by Andy on 2018-02-20.
//

#pragma once


class BehaviourTreeNode {
public:
    typedef enum {SUCCESS, FAILURE, RUNNING} STATUS;

    virtual STATUS tick() = 0;
};


