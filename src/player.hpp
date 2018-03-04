//
// Created by gowth on 2018-02-08.
//

#pragma once

#define NUM_DIRECTIONS 4

#include "common.hpp"

class Player : public Renderable {

    static Texture player_texture;

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

    //get position
    vec2 get_position() const;

    //set rotation
    void setRotation(float radians);


    void set_velocity(float velocity, DIRECTION dir);

    void swap_velocity(DIRECTION dir1, DIRECTION dir2);

    void setFlying(DIRECTION dir, bool isFlying);

    void destroy();

    float getRotation() const;

private:
    vec2 m_position; // Window coordinates
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    float m_rotation; // in radians
    vec2 m_velocity;
    bool m_isFlying[NUM_DIRECTIONS];
    float m_maxSpeed;
    size_t m_num_indices;
    int m_lives;

    float getMovementOrientation(DIRECTION dir);
    vec2 getNewVelocity(vec2 oldVelocity, vec2 delta);


};
