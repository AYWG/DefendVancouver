//
// Created by Tanwin Nun on 2018-04-11.
//

#include "state.hpp"

bool State::initTexture() {
    return false;
}

State::~State() {

}

State::State(World &world) : Entity(world) {

}

void State::setPosition(vec2 pos) {
    m_position.x = pos.x;
    m_position.y = pos.y;
}
