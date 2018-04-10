//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#pragma once

#include "UIobject.hpp"


class playerIcon : public UIobject, public Renderable{

public:

    static bool initGraphics();

    playerIcon(UI &ui);

    ~playerIcon() override;

    bool init();

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:
    static Graphics gfx;
    int frameCount;
    float frameWidth;
    float countdown;
    bool start;
    vec2 m_position;


};
