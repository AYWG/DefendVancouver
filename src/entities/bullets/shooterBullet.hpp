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
    ShooterBullet(World& world);

    static bool initTexture();

    static std::shared_ptr<ShooterBullet> spawn(World &world);

    ~ShooterBullet() override;

    bool init();

    void destroy() override;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    Region getBoundingBox() const override;

    unsigned int getMass() const override;
};


