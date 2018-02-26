//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#pragma once

#include <vector>
#include "../common.hpp"
#include "enemy.hpp"
#include "shooterBullet.hpp"
#include "../ai/shooterAI.hpp"

// Basic alien enemies for the game (grey spaceship)

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

    void attack() override;


private:
    ShooterAI m_ai;

    std::vector<ShooterBullet> m_shooterBullets;

    float m_nextShooterBulletSpawn;

    float m_rotation;

    bool spawnBullet();

    bool isPlayerInVision(vec2 playerPosition);

};