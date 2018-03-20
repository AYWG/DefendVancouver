//
// Created by Shrey Swades Nayak on 2018-03-19.
//

#ifndef DEFENDVANCOUVER_BOMBERBOMB_HPP
#define DEFENDVANCOUVER_BOMBERBOMB_HPP
#pragma once

#include "../../common.hpp"
#include "../entity.hpp"


class BomberBomb : public Entity, public Renderable {

    static Texture bomb_texture;
public:
    //init bomb
    bool init();

    int getFrameCount() const;

    void draw(const mat3 &projection) override;

    bool update(float ms);

    vec2 getBoundingBox() const;

    void animate();


private:
    TexturedVertex vertices[4];

    bool isHit;
    // true if bomb is hit
    int frameCount;

};

#endif //DEFENDVANCOUVER_BOMBERBOMB_HPP
