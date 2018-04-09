//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#pragma once


#include "../common.hpp"
#include "../world.hpp"

class UI {
public:
    UI(vec2 screenSize);

    bool init();

    void update(World *world, float ms);

    void draw();

private:

    vec2 m_screenSize;

};