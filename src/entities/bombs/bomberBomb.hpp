//
// Created by Shrey Swades Nayak on 2018-03-19.
//

#pragma once

#include <memory>
#include "../../common.hpp"
#include "../entity.hpp"


class BomberBomb : public Entity, public Renderable {

public:
    static bool initGraphics();

    static std::shared_ptr<BomberBomb> spawn(World &world);

    BomberBomb(World &world);

    bool init() override;

    void destroy() override;

    int getFrameCount() const;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    bool isBlasting();

    Region getBoundingBox() const override;

    std::string getName() const override;


private:
    static Graphics gfx;
    bool isHit;
    int frameCount;
    float countdown;
    float frameWidth;
    float frameHeight;

};
