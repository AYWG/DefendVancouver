//
// Created by Andy on 2018-03-08.
//

#pragma once


class Movable {
public:
    virtual ~Movable() = default;

    /**
     * Anything that is movable has a fixed mass.
     */
    virtual unsigned int getMass() const = 0;
};


