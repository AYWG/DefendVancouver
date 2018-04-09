//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#pragma once

#include <vector>
#include "enemy.hpp"
#include "../../ai/shooterAI.hpp"
#include "../bullets/shooterBullet.hpp"


class Shooter : public Enemy, public Renderable {

public:
    static bool initGraphics();

    static std::shared_ptr<Shooter> spawn(World &world);

    Shooter(World &world, ShooterAI &ai);

    bool init() override;

    void destroy() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    Region getBoundingBox() const override;

    void attack(float ms) override;

    int getPoints() const override;

    void shoot();

    unsigned int getMass() const override;

    std::string getName() const override;


private:
    static Graphics gfx;
    static int bulletDelayMS;
    float m_nextBulletSpawn;
};