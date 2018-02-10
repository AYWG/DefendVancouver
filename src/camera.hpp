//
// Created by Andy on 2018-02-08.
//

#pragma once

#include "common.hpp"

class Camera {
public:

    void setSize(vec2 size);

    vec2 getFocusPoint() const;

    void setFocusPoint(vec2 focusPoint);

    float getLeftBoundary() const;

    float getRightBoundary() const;

    float getTopBoundary() const;

    float getBottomBoundary() const;


private:
    // The dimensions of the camera - should be the same as the window
    // x : width, y : height
    vec2 m_size;

    // World coordinate that corresponds to the center of the camera
    vec2 m_focusPoint;
};


