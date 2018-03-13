//
// Created by Shrey Swades Nayak on 2018-02-26.
//

#ifndef DEFENDVANCOUVER_SPRITESHEET_HPP
#define DEFENDVANCOUVER_SPRITESHEET_HPP

#pragma once

#include "../common.hpp"
#include "entity.hpp"


class Bomb : public Entity, public Renderable{

    static Texture bomb_texture;
public:
    //init bomb
    bool init(const char *path);

    int getFrameCount()const;

    void draw(const mat3 &projection) override;

    bool update(float ms);

    vec2 getBoundingBox()const;

    void animate();


private:
    TexturedVertex vertices[4];

    bool isHit;
    // true if bomb is hit
    int frameCount;

};

#endif //DEFENDVANCOUVER_SPRITESHEET_HPP
