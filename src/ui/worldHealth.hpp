//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#pragma once

#include "UIobject.hpp"

class worldHealth : public UIobject, public Renderable{

public:

    static bool initGraphics();

    worldHealth(UI &ui);

    ~worldHealth() override;

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:
    static Graphics gfx;
    int health;
    int frameCount;
    int frameNumber;
    float frameWidth;
    float frameHeight;

};



