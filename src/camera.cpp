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

bool Camera::isEntityInView(const Entity &entity) const {
    auto entityRegion = entity.getBoundingBox();
    bool isWithinLeftBoundary = entityRegion.origin.x + entityRegion.size.x >= getLeftBoundary();
    bool isWithinRightBoundary = entityRegion.origin.x <= getRightBoundary();
    bool isWithinTopBoundary = entityRegion.origin.y + entityRegion.size.y >= getTopBoundary();
    bool isWithinBottomBoundary = entityRegion.origin.y <= getBottomBoundary();

    return isWithinLeftBoundary && isWithinRightBoundary && isWithinTopBoundary && isWithinBottomBoundary;
}

vec2 Camera::getFocusPoint() {
    return m_focusPoint;
}
