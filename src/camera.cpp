//
// Created by Andy on 2018-02-08.
//

#include "camera.hpp"

Camera::Camera(vec2 screenSize, vec2 worldSize) : m_screenSize(screenSize), m_worldSize(worldSize), m_focusPoint({0, 0}){}

float Camera::getLeftBoundary() const {
    return m_focusPoint.x - (m_screenSize.x / 2);
}

float Camera::getRightBoundary() const {
    return m_focusPoint.x + (m_screenSize.x / 2);
}

float Camera::getTopBoundary() const {
    return m_focusPoint.y - (m_screenSize.y / 2);
}

float Camera::getBottomBoundary() const {
    return m_focusPoint.y + (m_screenSize.y / 2);
}

void Camera::update(float ms, const vec2 &targetPos) {
    auto newCameraFocusPointX = std::min(m_worldSize.x - m_screenSize.x / 2, std::max(m_screenSize.x / 2, targetPos.x));
    auto newCameraFocusPointY = std::min(m_worldSize.y - m_screenSize.y / 2, std::max(m_screenSize.y / 2, targetPos.y));
    m_focusPoint = {newCameraFocusPointX, newCameraFocusPointY};
}