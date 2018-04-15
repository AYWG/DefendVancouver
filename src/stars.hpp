//
// Created by Shrey Swades Nayak on 2018-04-14.
//

#pragma once

#include "common.hpp"
#include "entities/entity.hpp"

class stars : public Renderable {

public:
    static bool initGraphics();

    stars();

    bool init();

    void draw(const mat3 &projection) override;

    void destroy();

private:
    static Graphics gfx;
    vec2 m_scale;
    vec2 m_position;
};
