//
// Created by gowth on 2018-02-08.
//

#pragma once

#define NUM_DIRECTIONS 4

#include <vector>
#include <list>
#include <random>
#include "../common.hpp"
#include "entity.hpp"
#include "movable.hpp"
#include "bullets/playerBullet.hpp"

class Player : public Entity, public Movable, public Renderable {

public:
    typedef enum {
        LEFT, FORWARD, RIGHT, BACKWARD
    } DIRECTION;

    Player(World &world);

    bool gameOver;

    //init ship
    bool init() override;

    // Renders the salmon
    void draw(const mat3 &projection) override;

    //move
    void move(vec2 off);

    //UPDATE
    void update(float ms) override;

    void setFlying(DIRECTION dir, bool isFlying);

    bool isShooting() const;

    void enableShooting(bool isShooting);

    void destroy() override;

    unsigned int getMass() const override;

    void shoot();

    Region getBoundingBox() const override;

    int getLives();

    void addLife();

    std::string getName() const override;

    void onCollision(Entity &entity) override;

    void takeDamage() override;

    bool isDamageable() const override;

    FACTION getFaction() const override;

    void setInvincibility(bool isInvincible);

    bool isCritical() const;

private:
    float getMovementOrientation(DIRECTION dir);

    vec2 getNewVelocity(vec2 oldVelocity, vec2 delta);

private:
    static int bulletDelayMS;

    FILE *mesh_file;

    int m_lives;
    vec2 m_velocity;
    bool m_isFlying[NUM_DIRECTIONS];
    float m_maxSpeed;
    size_t m_num_indices;
    std::vector<Vertex> vertices;

    bool m_isShootingEnabled;
    float m_nextBulletSpawn;
    float m_timeSinceLastBulletShot;

    bool m_isHit;
    float transparency;
    float countdown;

    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist{-1.f, 1.f};

    bool m_isInvincible;
};
