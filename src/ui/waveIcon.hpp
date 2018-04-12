//
// Created by Shrey Swades Nayak on 2018-04-11.
//

#pragma once

#include "UIobject.hpp"

class waveIcon : public UIobject, public Renderable{

public:

    static bool initGraphics();

    waveIcon(UI &ui);

    ~waveIcon() override;

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:
    static Graphics gfx;
    int frameCount;
    float frameWidth;
    float countdown;
    bool start;


};