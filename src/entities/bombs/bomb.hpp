//
// Created by Andy on 2018-04-08.
//

#pragma once


#include "../entity.hpp"

class Bomb : public Entity {
public:
    Bomb(World &world);

    void takeDamage() override;

    bool isDamageable() const override;

protected:
    void explode();

    bool isInvulnerable() const;


protected:
    bool m_isHit;
    float m_invulnerabilityCountdown;
    float m_explosionCountdown;
    float m_initCountdown;


};


