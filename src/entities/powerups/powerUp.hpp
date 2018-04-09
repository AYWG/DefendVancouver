//
// Created by Andy on 2018-04-08.
//

#pragma once


#include "../entity.hpp"

class PowerUp : public Entity {
public:
    PowerUp(World &world);

    void update(float ms) override;

    void takeDamage() override;

    bool isDamageable() const override;

    FACTION getFaction() const override;
};


