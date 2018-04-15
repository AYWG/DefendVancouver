//
// Created by Andy on 2018-03-07.
//

#pragma once

#include "../common.hpp"
#include "../region.hpp"
class World;

class Entity {

public:
    typedef enum {
        HUMAN, ALIEN, NONE
    } FACTION;

    Entity(World &world);

    virtual ~Entity() = default;

    virtual bool init() = 0;

    virtual void update(float ms) = 0;

    virtual void draw(const mat3 &projection) = 0;

    vec2 getPosition() const;

    void setPosition(vec2 position);

    float getRotation() const;

    void setRotation(float radians);

    vec2 getPlayerPosition() const;

    virtual void destroy() = 0;

    virtual Region getBoundingBox() const = 0;

    virtual bool isCollidingWith(Entity &other) const;

    virtual void onCollision(Entity &other) = 0;

    virtual void takeDamage() = 0;

    virtual bool isDamageable() const = 0;

    virtual FACTION getFaction() const = 0;

    void die();

    bool isDead() const;

    virtual std::string getName() const = 0;

protected:
    World *m_world;
    vec2 m_position;
    vec2 m_scale;
    float m_rotation;
    bool m_isDead;

};


