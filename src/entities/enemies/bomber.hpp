//
// Created by Shrey Swades Nayak on 2018-02-08.
//
#pragma once

#include "../../common.hpp"
#include "enemy.hpp"
#include "../../ai/bomberAI.hpp"

class Bomber : public Enemy, public Renderable {

    static Texture bomber_texture;

public:
    static bool initTexture();

    static std::shared_ptr<Bomber> spawn(World &world);

    Bomber(World &world, BomberAI &ai);

    ~Bomber() override;

    bool init() override;

    void destroy() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    Region getBoundingBox() const override;

    unsigned int getMass() const override;

    void attack(float ms) override;

    std::string getName() const override;
};