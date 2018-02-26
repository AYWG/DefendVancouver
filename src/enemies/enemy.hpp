//
// Created by Andy on 2018-02-19.
//

#pragma once


#include "../common.hpp"
class World;

class Enemy {
public:
    Enemy();

    virtual bool init() = 0;

    virtual void destroy() = 0;

    virtual void update(World *world, float ms) = 0;

    virtual vec2 getBoundingBox() const = 0;

    vec2 getPosition() const;

    void setPosition(vec2 position);

    /**
     * Every enemy has a different attack.
     */
    virtual void attack() = 0;

protected:
    vec2 m_position;
    vec2 m_scale;
    float m_rotation;
};


