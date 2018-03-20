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
#include "enemies/shooter.hpp"
#include "enemies/bomber.hpp"

class Player : public Entity, public Movable, public Renderable {

    static int bulletDelayMS;

    FILE *mesh_file;

public:
    typedef enum {
        LEFT, FORWARD, RIGHT, BACKWARD
    } DIRECTION;

    //init ship
    bool init(vec2 worldSize);

    // Renders the salmon
    void draw(const mat3 &projection) override;

    //move
    void move(vec2 off);

    //UPDATE
    void update(float ms);

    //set rotation
    void setRotation(float radians);

    float getRotation();

    void setFlying(DIRECTION dir, bool isFlying);

    void enableShooting(bool isShooting);

    void destroy();

    unsigned int getMass() const override;

    void shoot();

    std::vector<std::shared_ptr<PlayerBullet>> &getBullets();

    vec2 getVelocity() const;

    int getLives();

    void hit();

    bool collisionCheck(Shooter shooter);

    bool collisionCheck(Bomber &bomber);

    bool collisionCheck(ShooterBullet sb);

private:
    float m_rotation; // in radians
    vec2 m_velocity;
    bool m_isFlying[NUM_DIRECTIONS];
    float m_maxSpeed;
    size_t m_num_indices;
    int m_lives;
    bool m_isShootingEnabled;
    float m_nextBulletSpawn;
    float m_timeSinceLastBulletShot;
    vec2 m_worldSize;

    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist{-1.f, 1.f};
    std::vector<std::shared_ptr<PlayerBullet>> m_bullets;

    float getMovementOrientation(DIRECTION dir);

    vec2 getNewVelocity(vec2 oldVelocity, vec2 delta);
};
