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

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:
    static Graphics gfx;
    int frameCount;
    int frameNumber;
    float frameWidth;


};
