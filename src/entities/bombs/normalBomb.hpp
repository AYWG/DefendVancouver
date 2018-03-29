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

    static std::shared_ptr<NormalBomb> spawn();

    ~NormalBomb() override;

    bool init();

    void destroy() override;

    int getFrameCount() const;

    void draw(const mat3 &projection) override;

    bool update(float ms);

//    vec2 getBoundingBox() const;

    void animate();

    Region getBoundingBox() const override;

    std::string getName() const override;

private:
    TexturedVertex vertices[4];

    bool isHit;
    // true if bomb is hit
    int frameCount;

};

#endif //DEFENDVANCOUVER_SPRITESHEET_HPP
