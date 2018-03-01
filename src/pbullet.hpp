//
// Created by gowth on 2018-02-09.
//

#pragma once

#include "common.hpp"
#include "player.hpp"


class pBullet : public Renderable {
    static Texture pbullet_texture;

public:

    bool init();

    void draw(const mat3 &projection) override;

    void setPosition(vec2 position);

    void fireBullet(vec2 aimDir);

    void update(float ms);

    float m_velocity;
    bool is_Shotted = false;

    vec2 getPosition() const;

    vec2 getBoundingBox() const;

    void setDirection(vec2 direction);


private:
    vec2 m_scale;
    vec2 m_position;

    vec2 m_direction;

};
