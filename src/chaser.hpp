//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#include "common.hpp"

// Basic alien enemies for the game (grey spaceship)

class Chaser : public Renderable {

    static Texture chaser_texture;

public:

    bool init();

    void destroy();

    void update(float ms);

    void draw(const mat3& projection)override;

    vec2 get_position()const;

    void set_position(vec2 position);

//    vec2 get_bounding_box()const;

private:

    vec2 curr_pos;
    vec2 curr_scale;
    float curr_rotation;

};