//
// Created by gowth on 2018-02-09.
//

#pragma once


#include <memory>
#include "../../common.hpp"
#include "bullet.hpp"
#include "../enemies/shooter.hpp"
#include "../enemies/chaser.hpp"
#include "../enemies/bomber.hpp"
#include "../bombs/normalBomb.hpp"


class PlayerBullet : public Bullet, public Renderable {
    static Texture playerBulletTexture;

public:
    PlayerBullet(World &world);

    static bool initTexture();

    static std::shared_ptr<PlayerBullet> spawn(World &world);

    ~PlayerBullet() override;

    bool init();

    void destroy() override;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

//    vec2 getBoundingBox() const override;

    Region getBoundingBox() const override;

    unsigned int getMass() const override;

    std::string getName() const override;
};
