//
// Created by Tanwin Nun on 2018-04-11.
//
#pragma once

#include "../common.hpp"
#include "../entities/entity.hpp"

class State : public Entity, public Renderable {

public:

    static bool initGraphics();

    State(World &world);

    ~State();

    bool init() override;

    void setPosition(vec2 pos);

    void onCollision(Entity &other) override;

    void takeDamage() override ;

    bool isDamageable() const override;

    FACTION getFaction() const override;


};

