//
// Created by Shrey Swades Nayak on 2018-02-26.
//

#ifndef DEFENDVANCOUVER_SPRITESHEET_HPP
#define DEFENDVANCOUVER_SPRITESHEET_HPP

#pragma once

#include "../common.hpp"


class Bomb : public Renderable{

    static Texture bomb_texture;
public:
    //init bomb
    bool init(const char *path);

    vec2 get_position()const;

    int getFrameCount()const;

    void set_position(vec2 position);

    void draw(const mat3 &projection);

    bool update(float ms);

    void animate();


private:
    TexturedVertex vertices[4];

    vec2 b_position;
    bool isHit;
    vec2 b_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    // true if bomb is hit
    int frameCount;


};

#endif //DEFENDVANCOUVER_SPRITESHEET_HPP
