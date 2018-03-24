//
// Created by Andy on 2018-02-19.
//

#pragma once


#include <vector>
#include "../../common.hpp"
#include "../bombs/normalBomb.hpp"
#include "../entity.hpp"
#include "../movable.hpp"

class World;

class Enemy : public Entity, public Movable {
public:
    virtual bool init() = 0;

    virtual void update(World *world, float ms) = 0;

    virtual vec2 getBoundingBox() const = 0;

    float getAngleToTarget() const;

    void setAngleToTarget(float angle);

    void setBombsInVision(std::vector<vec2> &bombs);

    std::vector<vec2> getBombsInVision() const;

    /**
     * Every enemy has a different attack.
     */
    virtual void attack(float ms) = 0;

protected:
    float m_maxSpeed;
    float m_angleToTarget;

    std::vector<vec2> m_bombsInVision;
};


