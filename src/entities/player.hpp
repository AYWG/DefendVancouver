//
// Created by gowth on 2018-02-08.
//

#pragma once

#define NUM_DIRECTIONS 4

#include <vector>
#include <random>
#include "../common.hpp"
#include "entity.hpp"
#include "movable.hpp"
#include "bullets/playerBullet.hpp"

class Player : public Entity, public Movable, public Renderable {

    static int bulletDelayMS;

public:
    typedef enum {
        LEFT, FORWARD, RIGHT, BACKWARD
    } DIRECTION;

    //init ship
    bool init();

    // Renders the salmon
    void draw(const mat3& projection) override;

    //move
    void move(vec2 off);

    //UPDATE
    void update(float ms);

    //set rotation
    void setRotation(float radians);

    void set_velocity(float velocity, DIRECTION dir);

    void swap_velocity(DIRECTION dir1, DIRECTION dir2);

    void setFlying(DIRECTION dir, bool isFlying);

    void enableShooting(bool isShooting);

    void destroy();

    float getRotation() const;

    vec2 getVelocity() const;

    unsigned int getMass() const override;

    void shoot(float ms);

    std::vector<std::shared_ptr<PlayerBullet>>& getBullets();

private:
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    float m_rotation; // in radians
    vec2 m_velocity;
    bool m_isFlying[NUM_DIRECTIONS];
    float m_maxSpeed;
    size_t m_num_indices;
    int m_lives;
    bool m_isShootingEnabled;
    float m_nextBulletSpawn;
    float m_timeSinceLastBulletShot;

    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist{-1.f, 1.f};
    std::vector<std::shared_ptr<PlayerBullet>> m_bullets;

    float getMovementOrientation(DIRECTION dir);
    vec2 getNewVelocity(vec2 oldVelocity, vec2 delta);
};
