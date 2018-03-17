//
// Created by Andy on 2018-03-16.
//

#pragma once

#include "../common.hpp"

/**
 * Crosshair for the player's cursor
 */

class Crosshair : public Renderable {
    static Texture crosshairTexture;

public:
    Crosshair();

    bool init();

    void draw(const mat3 &projection) override;

    void setPosition(vec2 position);

private:
    vec2 m_position;
    vec2 m_scale;
};


