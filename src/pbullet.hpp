//
// Created by gowth on 2018-02-09.
//

#pragma once

#include "common.hpp"
#include "player.hpp"



class Pbullet : public Renderable {
    static Texture pbullet_texture;

public:

    bool init();
    void draw(const mat3& projection)override;
    void set_position(vec2 position);
    void fireBullet(vec2 aimDir);
    void update(float ms);
    float m_velocity;
    bool is_Shotted = false;
    vec2 get_position() const;
    vec2 get_bounding_box() const;


private:
    vec2 m_scale;
    vec2 m_position;
    Player m_player;











};
