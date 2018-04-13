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

    void setPosition(vec2 pos);

};


#endif //DEFENDVANCOUVER_STATE_HPP
