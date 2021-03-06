//
// Created by Shrey Swades Nayak on 2018-04-14.
//

#pragma once

#include <vector>
#include <memory>
#include "UIobject.hpp"
#include "Digit.hpp"

class GameOverUI : public UIobject, public Renderable{
public:

    static bool initGraphics();

    GameOverUI(UI &ui);

    ~GameOverUI() override;

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:

    std::vector<std::shared_ptr<Digit>> m_score;
    std::vector<std::shared_ptr<Digit>> m_highscore;

    int highscore;
    int score;
};