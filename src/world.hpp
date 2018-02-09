//
// Created by Tanwin Nun on 2018-02-07.
//

#pragma once

// stlib
#include <vector>
#include <random>
#include "common.hpp"

class world {
public:
    world();

    ~world();

    // Creates a window, sets up events and begins the game
    bool init(vec2 screen);

    // Releases all associated resources
    void destroy();

    // Steps the game ahead by ms milliseconds
    bool update(float ms);

    // Renders our scene
    void draw();

    // Should the game be over ?
    bool is_over() const;
};


