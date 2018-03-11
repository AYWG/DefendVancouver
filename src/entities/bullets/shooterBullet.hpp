//
// Created by Andy on 2018-02-24.
//

#pragma once


#include <memory>
#include "../../common.hpp"
#include "bullet.hpp"

class ShooterBullet : public Bullet, public Renderable {
    static Texture shooterBulletTexture;

public:
    static std::shared_ptr<ShooterBullet> spawn();

    bool init();

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    vec2 getBoundingBox() const override;

    unsigned int getMass() const override;
};


