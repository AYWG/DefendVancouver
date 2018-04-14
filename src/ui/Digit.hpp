//
// Created by Shrey Swades Nayak on 2018-04-13.
//

#pragma once

#include "UIobject.hpp"

class Digit : public UIobject, public Renderable{
public:

    static bool initGraphics();

    Digit(UI &ui);

    ~Digit() override;

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    void setDigit(int digit);

private:
    static Graphics gfx;
    int frameCount;
    int frameNumber;
    float frameWidth;


};
