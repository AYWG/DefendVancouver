//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#include "common.hpp"

// Basic alien enemies for the game (grey spaceship)

class BasicEnemy : public Renderable {

    static Texture basicEnemy_texture;

public:

    bool init();

    void destroy();

    void update(float ms);

    void draw(const mat3& projection)override;

    vec2 get_position()const;

    void set_position(vec2 position);

    vec2 get_bounding_box()const;

//    vec2 get_bounding_box()const;

private:

    vec2 m_pos;
    vec2 m_scale;
    float m_rotation;

};