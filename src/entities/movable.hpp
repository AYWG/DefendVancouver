//
// Created by Andy on 2018-03-08.
//

#pragma once


class Movable {
public:
    virtual ~Movable() = default;
    virtual unsigned int getMass() = 0;
};


