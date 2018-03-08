//
// Created by gowth on 2018-02-09.
//

#pragma once

#include "../common.hpp"
#include "bullet.hpp"


class PlayerBullet : public Bullet, public Renderable {
    static Texture playerBulletTexture;

public:
    bool init();

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    vec2 getBoundingBox() const override;
};
