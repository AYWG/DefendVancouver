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
    auto pIcon = std::make_shared<playerIcon>(*this);
    if (pIcon->init()) {
        m_objects.emplace_back(pIcon);
    }

    vec2 wavePos = {0,0};
    auto wIcon = std::make_shared<waveIcon>(*this);
    if (wIcon->init()) {
        wIcon->setPosition({UIwidth-75.f,70.f});
        m_objects.emplace_back(wIcon);
        wavePos = wIcon->getPosition();
    }

    auto enemyIndicator = std::make_shared<EnemyIndicator>(*this);
    if (enemyIndicator->init()) {
        m_objects.emplace_back(enemyIndicator);
    }

    auto healthBar = std::make_shared<worldHealth>(*this);
    if (healthBar->init()) {
        healthBar->setPosition({wavePos.x-125.f, wavePos.y});
        m_objects.emplace_back(healthBar);
    }

    auto playerLifeBar = std::make_shared<playerLives>(*this);
    if(playerLifeBar->init()){
        m_objects.emplace_back(playerLifeBar);
    }

    auto scoreBar = std::make_shared<Score>(*this);
    if(scoreBar->init()){
        m_objects.emplace_back(scoreBar);
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

int UI::getPlayerLives() const {
    return m_world->getPlayerLives();
}

int UI::getWorldHealth() const {
    return m_world->getWorldHealth();
}

int UI::getWave() const {
    return m_world->getWave();
}

int UI::getScore() const {
    return m_world->getScore();
}

bool UI::getInvincibility(){
    return m_world->getInvincibility();
}

bool UI::initGraphics() {
    return playerIcon::initGraphics() &&
           EnemyIndicator::initGraphics() &&
           worldHealth::initGraphics() &&
           waveIcon::initGraphics() &&
           playerLives::initGraphics() &&
           Score::initGraphics();
}

