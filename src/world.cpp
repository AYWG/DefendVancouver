//
// Created by Tanwin Nun on 2018-02-07.
//

// Header
#include "world.hpp"

// stlib
#include <iostream>
#include <string.h>
#include <cassert>
#include <sstream>
#include <math.h>


// Same as static in c, local to compilation unit
namespace {

}

World::World() {

}

World::~World() {

}

// World initialization
bool World::init(vec2 screen) {

    return m_player.init();
}

// Releases all the associated resources
void World::destroy() {

}

// Update our game world
bool World::update(float elapsed_ms) {

}

// Render our game world
void World::draw() {

}

// Should the game be over ?
bool World::is_over() const {

}

void on_key(GLFWwindow*, int key, int, int action, int mod){

}


void on_mouse_move(GLFWwindow* window, double xpos, double ypos){

}
