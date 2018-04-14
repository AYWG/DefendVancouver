//
// Created by Shrey Swades Nayak on 2018-04-13.
//

#include "playerLives.hpp"
#include "UI.hpp"

playerLives::playerLives(UI &ui) : UIobject(ui) {}

playerLives::~playerLives() {
    destroy();
}

bool playerLives::initGraphics() {
    return Digit::initGraphics();
}

bool playerLives::init() {
    auto firstDigit = std::make_shared<Digit>(*m_ui);
    if (firstDigit->init()) {
        firstDigit->setPosition({135.f, 70.f});
        m_digits.emplace_back(firstDigit);
    }
    auto secondDigit = std::make_shared<Digit>(*m_ui);
    if (secondDigit->init()) {
        secondDigit->setPosition({firstDigit->getPosition().x+30.f, firstDigit->getPosition().y});
        m_digits.emplace_back(secondDigit);
    }

    return true;
}

void playerLives::destroy() {
    for (auto &m_digit : m_digits) {
        m_digit->destroy();
    }
}

void playerLives::draw(const mat3 &projection) {

    for (auto &m_digit : m_digits) {
        m_digit->draw(projection);
    }
}

void playerLives::update(float ms) {
    lives = m_ui->getPlayerLives();

    for(int i = 1; i>=0; i--){
        m_digits[i]->setDigit(lives % 10);
        lives /= 10;
    }
}