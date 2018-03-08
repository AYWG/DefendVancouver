//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#pragma once

#include <vector>
#include "enemy.hpp"
#include "../../ai/shooterAI.hpp"
#include "../bullets/shooterBullet.hpp"


class Shooter : public Enemy, public Renderable {

    static Texture shooterTexture;
    static int maxNumberOfBullets;
    static int bulletDelayMS;

public:

    explicit Shooter(ShooterAI& ai);

    bool init() override;

    void destroy() override;

    void update(World *world, float ms) override;

    void draw(const mat3 &projection) override;

    vec2 getBoundingBox() const override;

    void attack(float ms) override;

    unsigned int getMass() const override;


private:
    ShooterAI m_ai;


//    vec2 get_bounding_box()const;

    std::vector<ShooterBullet> m_bullets;


    float m_nextBulletSpawn;

    bool spawnBullet();

};