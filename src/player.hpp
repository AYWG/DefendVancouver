//
// Created by gowth on 2018-02-08.
//

#pragma once

#include "common.hpp"

class player : public Renderable{

    static Texture player_texture;

public:
    //init ship
    bool init();

    // Renders the salmon
    void draw(const mat3& projection)override;

private:
    vec2 m_position; // Window coordinates
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    float m_rotation; // in radians


};
