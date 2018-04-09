//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#pragma once


#include "../common.hpp"
#include "../world.hpp"
#include "UIobject.hpp"

class UI {
public:
    UI(vec2 screenSize);

    bool init();

    void update(World *world, float ms);

    void draw(const mat3 &projection);

private:

    std::vector<std::shared_ptr<UIobject>> m_uiobjects;

    vec2 m_screenSize;

};