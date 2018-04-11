//
// Created by Andy on 2018-04-11.
//

#pragma once

#include "UIobject.hpp"

/**
 * An arrow that points to the nearest enemy if there are no enemies on screen
 */

class EnemyIndicator : public UIobject, public Renderable {
public:
    static bool initGraphics();

    EnemyIndicator(UI &ui);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:
    static Graphics gfx;

};


