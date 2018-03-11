//
// Created by Andy on 2018-03-07.
//

#pragma once


#include "../common.hpp"

class Entity {

public:
    Entity();
    virtual ~Entity() = default;

    vec2 getPosition() const;

    void setPosition(vec2 position);

protected:
    vec2 m_position;

};


