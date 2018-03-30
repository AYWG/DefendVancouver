//
// Created by Andy on 2018-03-03.
//

#pragma once


#include "../../common.hpp"
#include "../movable.hpp"
#include "../entity.hpp"

class Bullet : public Entity, public Movable {
public:
    Bullet(World &world);

    void setVelocity(vec2 velocity);

protected:
    vec2 m_velocity;
};


