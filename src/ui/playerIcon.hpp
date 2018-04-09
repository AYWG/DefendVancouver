//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#pragma once

#include "UIobject.hpp"


class playerIcon : public UIobject, public Renderable{

    static Texture playerIconTexture;

public:

    static bool initTexture();

    playerIcon(UI &ui);

    ~playerIcon() override;

    bool init();

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

private:
    TexturedVertex vertices[4];

};
