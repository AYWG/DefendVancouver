//
// Created by Shrey Swades Nayak on 2018-04-09.
//

#pragma once

#include "../common.hpp"

class UI;


class UIobject {
public:
    UIobject(UI &ui);

    virtual ~UIobject() = default;

    virtual bool init() = 0;

    virtual void update(float ms) = 0;

    virtual void draw(const mat3 &projection) = 0;

    vec2 getPosition() const;

    void setPosition(vec2 position);

    vec2 getScale() const;

    void setScale(vec2 scale);

    virtual void destroy() = 0;

protected:
    UI *m_ui;
    vec2 m_position;
    vec2 m_scale;
    float m_rotation;

    bool m_isHidden;

};



