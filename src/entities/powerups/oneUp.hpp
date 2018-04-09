//
// Created by Shrey Swades Nayak on 2018-03-26.
//

#pragma once

#include <memory>
#include "../../common.hpp"
#include "powerUp.hpp"

class OneUp : public PowerUp, public Renderable{

public:

    static bool initGraphics();

    static std::shared_ptr<OneUp> spawn(World &world);

    OneUp(World &world);

    bool init() override;

    void destroy() override;

    void draw(const mat3 &projection) override;

    Region getBoundingBox() const override;

    std::string getName() const override;

    void onCollision(Entity &other) override;


private:
    static Graphics gfx;
};

