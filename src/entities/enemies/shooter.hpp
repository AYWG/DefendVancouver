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
    static int bulletDelayMS;

public:
    static bool initTexture();

    static std::shared_ptr<Shooter> spawn();

    explicit Shooter(ShooterAI &ai);

    ~Shooter() override;

    bool init() override;

    void destroy() override;

    void update(World *world, float ms) override;

    void draw(const mat3 &projection) override;

//    vec2 getBoundingBox() const override;

    Region getBoundingBox() const override;

    void attack(float ms) override;

    unsigned int getMass() const override;

    std::vector<std::shared_ptr<ShooterBullet>> &getBullets();


private:
    ShooterAI m_ai;


//    vec2 get_bounding_box()const;

    std::vector<std::shared_ptr<ShooterBullet>> m_bullets;


    float m_nextBulletSpawn;

};