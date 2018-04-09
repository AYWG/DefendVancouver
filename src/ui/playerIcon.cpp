//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#include "playerIcon.hpp"

Texture playerIcon::playerIconTexture;

playerIcon::playerIcon(UI &ui) : UIobject(ui) {}

playerIcon::~playerIcon() {
    destroy();
}

bool playerIcon::initTexture() {

}

bool playerIcon::init() {

}

void playerIcon::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    effect.release();
}

void playerIcon::draw(const mat3 &projection) {

}

void playerIcon::update(float ms) {

}