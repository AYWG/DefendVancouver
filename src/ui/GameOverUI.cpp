//
// Created by Shrey Swades Nayak on 2018-04-14.
//

#include "GameOverUI.hpp"
#include "UI.hpp"

GameOverUI::GameOverUI(UI &ui) : UIobject(ui) {}

GameOverUI::~GameOverUI() {
    destroy();
}

bool GameOverUI::initGraphics() {
    return Digit::initGraphics();
}

bool GameOverUI::init() {
    vec2 UIsize = m_ui->getScreenSize();
    auto firstDigitS = std::make_shared<Digit>(*m_ui);
    if (firstDigitS->init()) {
        m_score.emplace_back(firstDigitS);
    }
    auto secondDigitS = std::make_shared<Digit>(*m_ui);
    if (secondDigitS->init()) {
        m_score.emplace_back(secondDigitS);
    }
    auto thirdDigitS = std::make_shared<Digit>(*m_ui);
    if (thirdDigitS->init()) {
        m_score.emplace_back(thirdDigitS);
    }
    auto fourthDigitS = std::make_shared<Digit>(*m_ui);
    if (fourthDigitS->init()) {
        m_score.emplace_back(fourthDigitS);
    }
    auto fifthDigitS = std::make_shared<Digit>(*m_ui);
    if (fifthDigitS->init()) {
        m_score.emplace_back(fifthDigitS);
    }
    auto sixthDigitS = std::make_shared<Digit>(*m_ui);
    if (sixthDigitS->init()) {
        m_score.emplace_back(sixthDigitS);
    }
    auto seventhDigitS = std::make_shared<Digit>(*m_ui);
    if (seventhDigitS->init()) {
        m_score.emplace_back(seventhDigitS);
    }

    firstDigitS->setPosition({(UIsize.x/2)-90.f, 350.f});
    secondDigitS->setPosition({(UIsize.x/2)-60.f, 350.f});
    thirdDigitS->setPosition({(UIsize.x/2)-30.f, 350.f});
    fourthDigitS->setPosition({(UIsize.x/2), 350.f});
    fifthDigitS->setPosition({(UIsize.x/2)+30.f, 350.f});
    sixthDigitS->setPosition({(UIsize.x/2)+60.f, 350.f});
    seventhDigitS->setPosition({(UIsize.x/2)+90.f, 350.f});

    auto firstDigitH = std::make_shared<Digit>(*m_ui);
    if (firstDigitH->init()) {
        m_score.emplace_back(firstDigitH);
    }
    auto secondDigitH = std::make_shared<Digit>(*m_ui);
    if (secondDigitH->init()) {
        m_score.emplace_back(secondDigitH);
    }
    auto thirdDigitH = std::make_shared<Digit>(*m_ui);
    if (thirdDigitH->init()) {
        m_score.emplace_back(thirdDigitH);
    }
    auto fourthDigitH = std::make_shared<Digit>(*m_ui);
    if (fourthDigitH->init()) {
        m_score.emplace_back(fourthDigitH);
    }
    auto fifthDigitH = std::make_shared<Digit>(*m_ui);
    if (fifthDigitH->init()) {
        m_score.emplace_back(fifthDigitH);
    }
    auto sixthDigitH = std::make_shared<Digit>(*m_ui);
    if (sixthDigitH->init()) {
        m_score.emplace_back(sixthDigitH);
    }
    auto seventhDigitH = std::make_shared<Digit>(*m_ui);
    if (seventhDigitH->init()) {
        m_score.emplace_back(seventhDigitH);
    }

    firstDigitH->setPosition({firstDigitS->getPosition().x, firstDigitS->getPosition().y + 50.f});
    secondDigitH->setPosition({secondDigitS->getPosition().x, secondDigitS->getPosition().y + 50.f});
    thirdDigitH->setPosition({thirdDigitS->getPosition().x, thirdDigitS->getPosition().y + 50.f});
    fourthDigitH->setPosition({fourthDigitS->getPosition().x, fourthDigitS->getPosition().y + 50.f});
    fifthDigitH->setPosition({fifthDigitS->getPosition().x, (UIsize.y/2)});
    sixthDigitH->setPosition({sixthDigitS->getPosition().x, (UIsize.y/2)});
    seventhDigitH->setPosition({seventhDigitS->getPosition().x, (UIsize.y/2)});

    return true;
}

void GameOverUI::destroy() {
    for (auto &m_digit : m_score) {
        m_digit->destroy();
    }
    for (auto &m_digit : m_highscore) {
        m_digit->destroy();
    }
}

void GameOverUI::draw(const mat3 &projection) {

    for (auto &m_digit : m_score) {
        m_digit->draw(projection);
    }
    for (auto &m_digit : m_highscore) {
        m_digit->draw(projection);
    }
}

void GameOverUI::update(float ms) {
    highscore = m_ui->getBestScore();
    score = m_ui->getScore();

    for(int i = 6; i>=0; i--){
        m_highscore[i]->setDigit(highscore % 10);
        highscore /= 10;
    }

    for(int i = 6; i>=0; i--){
        m_score[i]->setDigit(score % 10);
        score /= 10;
    }
}
