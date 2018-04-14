//
// Created by Shrey Swades Nayak on 2018-04-09.
//

#include "UIobject.hpp"

UIobject::UIobject(UI &ui) : m_ui(&ui), m_position({0.f, 0.f}), m_scale({1.f, 1.f}), m_rotation(0.f), m_isHidden(false){}

vec2 UIobject::getPosition() const {
    return m_position;
}

void UIobject::setPosition(vec2 position) {
    m_position = position;
}

vec2 UIobject::getScale() const {
    return m_scale;
}

void UIobject::setScale(vec2 scale) {
    m_scale = scale;
}