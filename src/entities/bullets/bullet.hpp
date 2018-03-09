//
// Created by Andy on 2018-03-03.
//

#pragma once


#include "../../common.hpp"
#include "../movable.hpp"
#include "../entity.hpp"

class Bullet : public Entity, public Movable {
public:
    Bullet();

    virtual void update(float ms) = 0;

    virtual vec2 getBoundingBox() const = 0;

    void setVelocity(vec2 velocity);

protected:
    vec2 m_scale;

    vec2 m_velocity;
};


