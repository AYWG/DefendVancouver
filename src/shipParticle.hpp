//
// Created by gowth on 2018-04-10.
//
#pragma once

#include <vector>
#include "common.hpp"
#include "entities/entity.hpp"



class shipParticle : public Entity, public Renderable  {


public:

    std::vector<vec2> pos_buf;
    std::vector<float> rot_buf;

    bool alphaBlend = false;

    static bool initGraphics();

    static std::shared_ptr<shipParticle> spawn(World &world);

    shipParticle(World &world);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    void setPosition(vec2 position);



    Region getBoundingBox() const override;

    std::string getName() const override;

    void onCollision(Entity &other) override;

    void takeDamage() override;

    bool isDamageable() const override;

    FACTION getFaction() const override;





private:
    static Graphics gfx;
    float plife;
    vec2 m_position;
    vec2 m_scale;




};



