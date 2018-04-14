//
// Created by Anun Ganbat on 2018-02-09.
//


#pragma once

#include "common.hpp"
#include "entities/entity.hpp"

class background : public Entity, public Renderable {

public:
    static bool initGraphics();

    background(World &world);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    Region getBoundingBox() const override;

    int getHealth();

    void addHealth();

    void decreaseHealth();

    std::string getName() const override;

    void onCollision(Entity &other) override;

    void takeDamage() override;

    bool isDamageable() const override;

    FACTION getFaction() const override;

    void setInvincibility(bool isInvincible);

private:
    static Graphics gfx;
    int m_health;

    bool m_isInvincible;

};


