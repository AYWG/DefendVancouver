//
// Created by Shrey Swades Nayak on 2018-02-26.
//

#pragma once

#include <memory>
#include "../../common.hpp"
#include "bomb.hpp"


class NormalBomb : public Bomb, public Renderable {

public:
    static bool initGraphics();

    static std::shared_ptr<NormalBomb> spawn(World &world);

    NormalBomb(World &world);

    bool init() override;

    void destroy() override;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    void explode();

    Region getBoundingBox() const override;

    std::string getName() const override;

    FACTION getFaction() const override;

private:
    static Graphics gfx;

    int frameCount;
    float frameWidth;
    float frameHeight;


};
