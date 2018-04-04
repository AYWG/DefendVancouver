//
// Created by Shrey Swades Nayak on 2018-02-26.
//

#ifndef DEFENDVANCOUVER_SPRITESHEET_HPP
#define DEFENDVANCOUVER_SPRITESHEET_HPP

#pragma once

#include <memory>
#include "../../common.hpp"
#include "../entity.hpp"


class NormalBomb : public Entity, public Renderable {

    static Texture bomb_texture;

public:
    static bool initTexture();

    static std::shared_ptr<NormalBomb> spawn(World &world);

    NormalBomb(World &world);

    ~NormalBomb() override;

    bool init() override;

    void destroy() override;

    int getFrameCount() const;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    void animate();

    Region getBoundingBox() const override;

    std::string getName() const override;

private:
    TexturedVertex vertices[4];

    bool isHit;
    int frameCount;
    float frameWidth;
    float frameHeight;


};

#endif //DEFENDVANCOUVER_SPRITESHEET_HPP
