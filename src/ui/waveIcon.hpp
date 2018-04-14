//
// Created by Shrey Swades Nayak on 2018-04-11.
//

#pragma once

#include <vector>
#include <memory>
#include "UIobject.hpp"
#include "Digit.hpp"

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

    std::vector<std::shared_ptr<Digit>> m_digits;

    int frameCount;
    int frameNumber;
    float frameWidth;
    float countdown;
    bool start;

    int waveCount;


};