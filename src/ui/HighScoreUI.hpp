//
// Created by Shrey Swades Nayak on 2018-04-14.
//

#pragma once

#include <vector>
#include <memory>
#include "UIobject.hpp"
#include "Digit.hpp"

class HighScoreUI : public UIobject, public Renderable{
public:

    static bool initGraphics();

    HighScoreUI(UI &ui);

    ~HighScoreUI() override;

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:

    std::vector<std::shared_ptr<Digit>> m_digits;

    int highscore;
};