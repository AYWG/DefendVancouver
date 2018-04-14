//
// Created by Shrey Swades Nayak on 2018-04-13.
//

#pragma once

#include <vector>
#include <memory>
#include "UIobject.hpp"
#include "Digit.hpp"

class playerLives : public UIobject, public Renderable{
public:

    static bool initGraphics();

    playerLives(UI &ui);

    ~playerLives() override;

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:

    std::vector<std::shared_ptr<Digit>> m_digits;

    int lives;
};