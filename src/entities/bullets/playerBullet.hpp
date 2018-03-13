//
// Created by gowth on 2018-02-09.
//

#pragma once


#include <memory>
#include "../../common.hpp"
#include "bullet.hpp"
#include "../enemies/shooter.hpp"
#include "../enemies/chaser.hpp"
#include "../bomb.hpp"


class PlayerBullet : public Bullet, public Renderable {
    static Texture playerBulletTexture;

public:
    static std::shared_ptr<PlayerBullet> spawn();

    bool init();

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    vec2 getBoundingBox() const override;

    unsigned int getMass() const override;

    bool collisionCheck(Shooter shooter);
    bool collisionCheck(Chaser chaser);
   // bool collisionCheck(Bomber& bomber);
    bool collisionCheck(Bomb& bomb);
};
