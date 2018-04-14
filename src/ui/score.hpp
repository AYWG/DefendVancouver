//
// Created by Shrey Swades Nayak on 2018-04-13.
//

#pragma once

#include <vector>
#include <memory>
#include "UIobject.hpp"
#include "Digit.hpp"

class Score : public UIobject, public Renderable{
public:

    static bool initGraphics();

    Score(UI &ui);

    ~Score() override;

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:

    std::vector<std::shared_ptr<Digit>> m_digits;

    int score;

};

