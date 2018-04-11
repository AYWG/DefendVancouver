//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#include "UI.hpp"
#include "enemyIndicator.hpp"

UI::UI(vec2 screenSize) : m_screenSize(screenSize) {}

bool UI::init() {

    UIwidth = m_screenSize.x;
    UIheight = m_screenSize.y;

    initGraphics();
    auto icon = std::make_shared<playerIcon>(*this);
    if (icon->init()) {
        m_objects.emplace_back(icon);
    }

    auto enemyIndicator = std::make_shared<EnemyIndicator>(*this);
    if (enemyIndicator->init()) {
        m_objects.emplace_back(enemyIndicator);
    }

    return true;
}

void UI::update(float ms) {
    for (auto &object: m_objects) {
        object->update(ms);
    }

}

void UI::draw(const mat3 &projection) {
    for (auto &object: m_objects) {
        object->draw(projection);
    }
}

bool UI::initGraphics() {
    return playerIcon::initGraphics() &&
           EnemyIndicator::initGraphics();
}
