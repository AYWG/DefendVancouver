//
// Created by Andy on 2018-02-08.
//

#pragma once

#include "common.hpp"
#include "entities/entity.hpp"

class Camera {
public:
    Camera(vec2 screenSize, vec2 worldSize);

    float getLeftBoundary() const;

    float getRightBoundary() const;

    float getTopBoundary() const;

    float getBottomBoundary() const;

    void update(float ms, const vec2 &targetPos);

    bool isEntityInView(const Entity &entity) const;

    vec2 getFocusPoint();

private:
    // The dimensions of the camera - should be the same as the window
    // x : width, y : height
    vec2 m_screenSize;

    vec2 m_worldSize;

    // World coordinate that corresponds to the center of the camera
    vec2 m_focusPoint;
};


