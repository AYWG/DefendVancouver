//
// Created by Andy on 2018-04-08.
//

#pragma once


#include "../entity.hpp"

class Bomb : public Entity {
public:
    Bomb(World &world);

    void onCollision(Entity &other) override;

    void takeDamage() override;

    bool isDamageable() const override;

protected:
    bool m_isHit;

};


