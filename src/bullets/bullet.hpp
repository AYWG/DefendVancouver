//
// Created by Andy on 2018-03-03.
//

#pragma once


#include "../common.hpp"

class Bullet {
public:
    Bullet();

    virtual void update(float ms) = 0;

    vec2 getPosition() const;

    void setPosition(vec2 position);

    void setDirection(vec2 direction);

    virtual vec2 getBoundingBox() const = 0;

    void setSpeed(float speed);

protected:
    vec2 m_scale;
    vec2 m_position;

    // A unit vector representing the bullet's direction
    vec2 m_direction;

    float m_speed;
};


