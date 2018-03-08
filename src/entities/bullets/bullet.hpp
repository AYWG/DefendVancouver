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

    void setDirection(vec2 direction);

    virtual vec2 getBoundingBox() const = 0;

    void setSpeed(float speed);

protected:
    vec2 m_scale;

    // A unit vector representing the bullet's direction
    vec2 m_direction;

    float m_speed;
};


