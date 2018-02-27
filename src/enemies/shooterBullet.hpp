//
// Created by Andy on 2018-02-24.
//

#pragma once


#include "../common.hpp"

class ShooterBullet : public Renderable {
    static Texture shooterBulletTexture;

public:
    float m_velocity;

    bool init();

    void draw(const mat3 &projection) override;

    void fire(vec2 aimDirection);

    void update(float ms);

    vec2 getPosition() const;

    void setPosition(vec2 position);

    void setDirection(vec2 direction);

private:

    vec2 m_scale;
    vec2 m_position;

    // A unit vector representing the bullet's direction
    vec2 m_direction;

};


