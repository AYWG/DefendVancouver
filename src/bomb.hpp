//
// Created by Shrey Swades Nayak on 2018-02-26.
//

#ifndef DEFENDVANCOUVER_SPRITESHEET_HPP
#define DEFENDVANCOUVER_SPRITESHEET_HPP

#pragma once

#include "common.hpp"

class Bomb : public Renderable{

    static Texture bomb_texture;
public:
    //init bomb
    bool init(const char *path);

    vec2 get_position()const;

    void draw(const mat3 &projection);
private:
    vec2 b_position; // Window coordinates
    vec2 b_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    void set_position(vec2 position);

};

#endif //DEFENDVANCOUVER_SPRITESHEET_HPP
