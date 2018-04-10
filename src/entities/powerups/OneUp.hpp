//
// Created by Shrey Swades Nayak on 2018-03-26.
//

#pragma once

#include <memory>
#include "../entity.hpp"
#include "../../common.hpp"

class OneUp : public Entity, public Renderable{

public:

    static bool initGraphics();

    static std::shared_ptr<OneUp> spawn(World &world);

    OneUp(World &world);

    bool init() override;

    void destroy() override;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    Region getBoundingBox() const override;

    std::string getName() const override;

private:
    static Graphics gfx;
};

