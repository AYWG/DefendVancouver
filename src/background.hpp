//
// Created by Anun Ganbat on 2018-02-09.
//

#ifndef DEFENDVANCOUVER_BACKGROUND_H
#define DEFENDVANCOUVER_BACKGROUND_H

#pragma once

#include "common.hpp"
class background : public Renderable{

    static Texture background_texture;
public:
    //init ship
    bool init();
    //get position
    vec2 get_position()const;

    void draw(const mat3 &projection);
private:
    vec2 b_position; // Window coordinates
    vec2 b_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    void set_position(vec2 position);
};


#endif //DEFENDVANCOUVER_BACKGROUND_H
