//
// Created by Shrey Swades Nayak on 2018-02-08.
//
#pragma once

#include "../../common.hpp"
#include "enemy.hpp"

class Bomber : public Enemy, public Renderable {

    static Texture bomber_texture;

public:
    static bool initTexture();

    static std::shared_ptr<Bomber> spawn();

    ~Bomber() override;

    bool init() override;

    void destroy() override;

    void update(World *world, float ms) override;

    void draw(const mat3 &projection) override;

    vec2 getBoundingBox() const override;

    Region getBoundingBox() override;

    unsigned int getMass() const override;

    void attack(float ms) override;
};