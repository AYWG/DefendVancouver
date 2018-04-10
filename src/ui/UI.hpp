//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#pragma once


#include <vector>
#include "../common.hpp"
#include "UIobject.hpp"
#include "playerIcon.hpp"

class UI {
public:
    UI(vec2 screenSize);

    bool init();

    void update(float ms);

    void draw(const mat3 &projection);

private:

    bool initGraphics();


private:
    /**
     * All UI objects. The playerIcon will always be the first object, followed by the healthbars.
     */

    std::vector<std::shared_ptr<UIobject>> m_objects;

    vec2 m_screenSize;

    float UIwidth;
    float UIheight;

};