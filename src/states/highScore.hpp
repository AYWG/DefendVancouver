//
// Created by Tanwin Nun on 2018-04-14.
//

#pragma once


#include "state.hpp"

class HighScore: public State {
public:

static bool initGraphics();

HighScore(World &world);

void update(float ms) override;

void draw(const mat3 &projection) override;

void destroy() override;

Region getBoundingBox() const override;

std::string getName() const override;

private:

static Graphics gfx;

};

