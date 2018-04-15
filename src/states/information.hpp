//
// Created by Tanwin Nun on 2018-04-12.
//

#pragma once

#include "state.hpp"

class Info: public State {
public:

    static bool initGraphics();

    Info(World &world);

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    Region getBoundingBox() const override;

    std::string getName() const override;

private:

    static Graphics gfx;

};

