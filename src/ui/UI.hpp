//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#pragma once


#include "../common.hpp"
#include "UIobject.hpp"

class UI {
public:
    UI(vec2 screenSize);

    bool init();

    void update(float ms);

    void draw(const mat3 &projection);

private:



    vec2 m_screenSize;

};