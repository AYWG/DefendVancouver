//
// Created by gowth on 2018-02-08.
//

#pragma once

#include "common.hpp"

class player : public Renderable{

    static Texture player_texture;

public:
    //init ship
    bool init();

    //move
    bool is_move()const;

    // Renders the salmon
    void draw(const mat3& projection)override;

    //move
    void move(vec2 off);

    //UPDATE
    void update(float ms);


    void isMoveUp(bool moveUp);
    void isMoveDwn(bool moveDwn);
    void isMoveLft(bool moveLft);
    void isMoveRht(bool moveRht);

private:
    vec2 m_position; // Window coordinates
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    float m_rotation; // in radians
    bool m_move_up;
    bool m_move_dwn;
    bool m_move_lft;
    bool m_move_rht;
    bool m_isMove;
    size_t m_num_indices;


};
