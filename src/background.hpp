//
// Created by Anun Ganbat on 2018-02-09.
//


#pragma once

#include "common.hpp"
#include "entities/entity.hpp"

class background : public Entity, public Renderable {

    static Texture background_texture;
public:
    static bool initTexture();

    background(World &world);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection);

    void destroy() override;

    Region getBoundingBox() const override;

    int getHealth();

    void addHealth();

    void decreaseHealth();

private:
    int m_health;

};


