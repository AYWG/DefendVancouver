//
// Created by Andy on 2018-02-19.
//

#pragma once


#include <vector>
#include "../common.hpp"
#include "../bomb.hpp"

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

    float getRotation() const;

    void setRotation(float rotation);

    float getAngleToTarget() const;

    void setAngleToTarget(float angle);

    void setBombsInVision(std::vector<vec2> &bombs);

    std::vector<vec2> getBombsInVision() const;

    /**
     * Every enemy has a different attack.
     */
    virtual void attack(float ms) = 0;

protected:
    vec2 m_position;
    vec2 m_scale;
    float m_rotation;
    float m_maxSpeed;

    float m_angleToTarget;

    std::vector<vec2> m_bombsInVision;
};


