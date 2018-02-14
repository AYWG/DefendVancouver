//
// Created by Andy on 2018-02-08.
//

#include "camera.hpp"

void Camera::setSize(vec2 size) {
    m_size = size;
}

vec2 Camera::getFocusPoint() const {
    return m_focusPoint;
}

void Camera::setFocusPoint(vec2 focusPoint) {
    m_focusPoint = focusPoint;
}

float Camera::getLeftBoundary() const {
    return m_focusPoint.x - (m_size.x / 2);
}

float Camera::getRightBoundary() const {
    return m_focusPoint.x + (m_size.x / 2);
}

float Camera::getTopBoundary() const {
    return m_focusPoint.y - (m_size.y / 2);
}

float Camera::getBottomBoundary() const {
    return m_focusPoint.y + (m_size.y / 2);
}
