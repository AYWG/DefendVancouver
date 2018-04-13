//
// Created by Tanwin Nun on 2018-04-11.
//

#include "gameOver.hpp"

bool GameOver::init() {
    return false;
}

void GameOver::update(float ms) {

}

void GameOver::draw(const mat3 &projection) {

}

void GameOver::destroy() {

}

Region GameOver::getBoundingBox() const {
    return Region();
}

std::string GameOver::getName() const {
    return std::__cxx11::string();
}

void GameOver::onKey(GLFWwindow *, int key, int, int action, int mod) {

}

void GameOver::onMouseMove(GLFWwindow *window, double xpos, double ypos) {

}

void GameOver::onMouseClick(GLFWwindow *window, int button, int action, int mod) {

}

bool GameOver::initTexture() {
    return false;
}

GameOver::GameOver(World &world) : State(world) {

}
