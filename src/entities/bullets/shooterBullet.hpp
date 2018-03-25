//
// Created by Andy on 2018-02-24.
//

#pragma once


#include <memory>
#include "../../common.hpp"
#include "bullet.hpp"
#include "../bombs/normalBomb.hpp"


class ShooterBullet : public Bullet, public Renderable {
    static Texture shooterBulletTexture;

public:
    static bool initTexture();

    static std::shared_ptr<ShooterBullet> spawn();

    ~ShooterBullet() override;

    bool init();

    void destroy() override;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    vec2 getBoundingBox() const override;

    Region getBoundingBox() override;

    unsigned int getMass() const override;

    bool collisionCheck(NormalBomb &bomb);
};


