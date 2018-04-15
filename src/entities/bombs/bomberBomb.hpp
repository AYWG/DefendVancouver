//
// Created by Shrey Swades Nayak on 2018-03-19.
//

#pragma once

#include <memory>
#include "../../common.hpp"
#include "bomb.hpp"


class BomberBomb : public Bomb, public Renderable {

public:
    static bool initGraphics();

    static std::shared_ptr<BomberBomb> spawn(World &world);

    BomberBomb(World &world);

    bool init() override;

    void destroy() override;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    Region getBoundingBox() const override;

    std::string getName() const override;

    FACTION getFaction() const override;

    void onCollision(Entity &other) override;

private:
    static Graphics gfx;

    int frameCount;
    int frameNumber;
    float countdown;
    float frameWidth;
    float frameHeight;

};
