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
        UP, DOWN, LEFT, RIGHT
    } DIRECTION;

    //init ship
    bool init();

    //move
    bool is_move() const;

    void draw(const mat3 &projection) override;

    //move
    void move(vec2 off);

    //UPDATE
    void update(float ms);

    //get position
    vec2 get_position() const;

    //set rotation
    void set_rotation(float radians);


    void set_velocity(float velocity, DIRECTION dir);

    void swap_velocity(DIRECTION dir1, DIRECTION dir2);

    void set_flying(bool is_flying, DIRECTION dir);

    float get_max_speed() const;

private:
    vec2 m_position; // Window coordinates
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    vec2 up_position;
    float m_rotation; // in radians
    bool m_isMove;
    float m_velocity[NUM_DIRECTIONS];
    bool m_is_flying[NUM_DIRECTIONS];
    float m_max_speed;
    size_t m_num_indices;


};
