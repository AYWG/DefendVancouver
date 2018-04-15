//
// Created by Andy on 2018-02-19.
//

#pragma once


#include <vector>
#include <memory>
#include "../../common.hpp"
#include "../entity.hpp"
#include "../movable.hpp"

class AI;

class Enemy : public Entity, public Movable {
public:
    Enemy(World &world, AI &ai);

    virtual ~Enemy() = default;

    float getAngleToTarget() const;

    void setAngleToTarget(float angle);

    void setBombsInVision(std::vector<vec2> &bombs);

    std::vector<vec2> getBombsInVision() const;

    vec2 getPlayerPosition() const;

    std::vector<vec2> getBombPositions() const;

    vec2 getCityPosition() const;

    bool isInView() const;

    /**
     * Every enemy has a different attack.
     */
    virtual void attack(float ms) = 0;

    /**
     * Every enemy is worth some number of points
     */
    virtual int getPoints() const = 0;

    void onCollision(Entity &other) override;

    void takeDamage() override;

    bool isDamageable() const override;

    FACTION getFaction() const override;

protected:
    std::unique_ptr<AI> m_ai;

    float m_angleToTarget;

    std::vector<vec2> m_bombsInVision;
};


