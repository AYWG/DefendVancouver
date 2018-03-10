//
// Created by gowth on 2018-02-09.
//

#pragma once

#include <memory>

#include "../../common.hpp"
#include "bullet.hpp"

using std::unique_ptr;

class PlayerBullet : public Bullet, public Renderable {
    static Texture playerBulletTexture;

public:
    static PlayerBullet* spawn();

    bool init();

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    vec2 getBoundingBox() const override;

    unsigned int getMass() const override;
};
