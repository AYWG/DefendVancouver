//
// Created by Shrey Swades Nayak on 2018-03-19.
//

#ifndef DEFENDVANCOUVER_BOMBERBOMB_HPP
#define DEFENDVANCOUVER_BOMBERBOMB_HPP
#pragma once

#include <memory>
#include "../../common.hpp"
#include "../entity.hpp"


class BomberBomb : public Entity, public Renderable {

    static Texture bomb_texture;
public:
    static bool initTexture();

    static std::shared_ptr<BomberBomb> spawn(World &world);

    BomberBomb(World &world);

    ~BomberBomb() override;

    bool init();

    void destroy() override;

    int getFrameCount() const;

    void draw(const mat3 &projection) override;

    void update(float ms) override;

    bool isBlasting();

    Region getBoundingBox() const override;

    std::string getName() const override;


private:
    TexturedVertex vertices[4];

    bool isHit;
    int frameCount;
    float countdown;
    float frameWidth;
    float frameHeight;

};

#endif //DEFENDVANCOUVER_BOMBERBOMB_HPP
