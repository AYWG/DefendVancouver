//
// Created by Shrey Swades Nayak on 2018-02-08.
//
#pragma once

#include "../common.hpp"
#include "enemy.hpp"

// Basic alien enemies for the game (grey spaceship)

class Chaser : public Enemy, public Renderable {

    static Texture chaser_texture;

public:

    bool init() override;

    void destroy() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    //    vec2 getBoundingBox()const;

};