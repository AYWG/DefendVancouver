//
// Created by Tanwin Nun on 2018-04-11.
//

#ifndef DEFENDVANCOUVER_STATE_HPP
#define DEFENDVANCOUVER_STATE_HPP


#include "../common.hpp"
#include "../entities/entity.hpp"

class State : public Entity, public Renderable {

public:

    static Texture state_texture;

    static bool initTexture();

    State(World &world);

    ~State();

    bool init() override;

    void setPosition(vec2 pos);

    void onCollision(Entity &other) override;

    void takeDamage() override ;

    bool isDamageable() const override;

    FACTION getFaction() const override;


};


#endif //DEFENDVANCOUVER_STATE_HPP
