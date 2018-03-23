//
// Created by Anun Ganbat on 2018-02-09.
//


#pragma once

#include "common.hpp"
#include "entities/entity.hpp"

class background : public Entity, public Renderable {

    static Texture background_texture;
public:
    static bool initTexture();

    bool init();

    void draw(const mat3 &projection);
};


