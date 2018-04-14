//
// Created by Shrey Swades Nayak on 2018-04-13.
//

#include "score.hpp"
#include "UI.hpp"

Score::Score(UI &ui) : UIobject(ui) {}

Score::~Score() {
    destroy();
}

bool Score::initGraphics() {
    return Digit::initGraphics();
}

bool Score::init() {
    vec2 UIsize = m_ui->getScreenSize();
    auto firstDigit = std::make_shared<Digit>(*m_ui);
    if (firstDigit->init()) {
        m_digits.emplace_back(firstDigit);
    }
    auto secondDigit = std::make_shared<Digit>(*m_ui);
    if (secondDigit->init()) {
        m_digits.emplace_back(secondDigit);
    }
    auto thirdDigit = std::make_shared<Digit>(*m_ui);
    if (thirdDigit->init()) {
        m_digits.emplace_back(thirdDigit);
    }
    auto fourthDigit = std::make_shared<Digit>(*m_ui);
    if (fourthDigit->init()) {
        m_digits.emplace_back(fourthDigit);
    }
    auto fifthDigit = std::make_shared<Digit>(*m_ui);
    if (fifthDigit->init()) {
        m_digits.emplace_back(fifthDigit);
    }
    auto sixthDigit = std::make_shared<Digit>(*m_ui);
    if (sixthDigit->init()) {
        m_digits.emplace_back(sixthDigit);
    }
    auto seventhDigit = std::make_shared<Digit>(*m_ui);
    if (seventhDigit->init()) {
        m_digits.emplace_back(seventhDigit);
    }

    firstDigit->setPosition({(UIsize.x/2)-90.f, 70.f});
    secondDigit->setPosition({(UIsize.x/2)-60.f, 70.f});
    thirdDigit->setPosition({(UIsize.x/2)-30.f, 70.f});
    fourthDigit->setPosition({(UIsize.x/2), 70.f});
    fifthDigit->setPosition({(UIsize.x/2)+30.f, 70.f});
    sixthDigit->setPosition({(UIsize.x/2)+60.f, 70.f});
    seventhDigit->setPosition({(UIsize.x/2)+90.f, 70.f});

    return true;
}

void Score::destroy() {
    for (auto &m_digit : m_digits) {
        m_digit->destroy();
    }
}

void Score::draw(const mat3 &projection) {

    for (auto &m_digit : m_digits) {
        m_digit->draw(projection);
    }
}

void Score::update(float ms) {
    score = m_ui->getScore();

    for(int i = 6; i>=0; i--){
        m_digits[i]->setDigit(score % 10);
        score /= 10;
    }

}
