//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#include "UI.hpp"
#include "../world.hpp"


UI::UI(vec2 screenSize, World &world) : m_screenSize(screenSize), m_world(&world) {}

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

    auto healthBar = std::make_shared<worldHealth>(*this);
    if (healthBar->init()) {
        m_objects.emplace_back(healthBar);
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

vec2 UI::getPlayerScreenPosition() const {
    return m_world->getPlayerScreenPosition();
}

vec2 UI::getPlayerPosition() const {
    return m_world->getPlayerPosition();
}

vec2 UI::getNearestEnemyPosToPlayer() const {
    return m_world->getNearestEnemyPosToPlayer();
}

bool UI::isOffScreenEnemyPresentAndNoEnemiesVisible() const {
    return m_world->isOffScreenEnemyPresentAndNoEnemiesVisible();
}

vec2 UI::getScreenSize() const {
    return m_screenSize;
}

bool UI::initGraphics() {
    return playerIcon::initGraphics() &&
           EnemyIndicator::initGraphics() &&
           worldHealth::initGraphics();
}
