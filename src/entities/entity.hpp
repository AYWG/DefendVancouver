//
// Created by Andy on 2018-03-07.
//

#pragma once


#include "../common.hpp"
#include "../region.hpp"

class Entity {

public:
    Entity();

    virtual ~Entity() = default;

    vec2 getPosition() const;

    void setPosition(vec2 position);

    float getRotation() const;

    void setRotation(float radians);

    virtual void destroy() = 0;

    virtual Region getBoundingBox() = 0;

protected:
    vec2 m_position;
    vec2 m_scale;
    float m_rotation;

};


