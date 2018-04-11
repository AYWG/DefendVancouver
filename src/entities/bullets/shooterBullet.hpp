//
// Created by Andy on 2018-02-24.
//

#pragma once


#include <memory>
#include "../../common.hpp"
#include "bullet.hpp"
#include "../bombs/normalBomb.hpp"


class ShooterBullet : public Bullet, public Renderable {

public:
    ShooterBullet(World& world);

    static bool initGraphics();

    static std::shared_ptr<ShooterBullet> spawn(World &world);

    bool init() override;

    void destroy() override;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    Region getBoundingBox() const override;

    unsigned int getMass() const override;

    std::string getName() const override;

    FACTION getFaction() const override;

private:
    static Graphics gfx;
};


