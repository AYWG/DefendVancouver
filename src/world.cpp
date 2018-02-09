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

    if (!glfwInit()) {
        // Handle initialization failure
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //GLFWwindow *window = glfwCreateWindow(1280, 720, "DefendVancouver", NULL, NULL);
    m_window = glfwCreateWindow((int)screen.x, (int)screen.y, "A1 Assignment", nullptr, nullptr);


    // Load OpenGL function pointers
    gl3wInit();

    int w, h;

    glfwGetFramebufferSize(m_window, &w, &h);

    glViewport(0, 0, w, h);
    glDepthRange(0.00001, 10);
    const float clear_color[3] = {0.3f, 0.3f, 0.8f};
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    while (!glfwWindowShouldClose(m_window)) {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }


    if (m_window == nullptr)
        return false;



    return m_player.init();
}

// Releases all the associated resources
void World::destroy() {

}

// Update our game world
bool World::update(float elapsed_ms) {

    m_player.update(elapsed_ms);

}

// Render our game world
void World::draw() {
    // Clearing error buffer
    gl_flush_errors();

    // Getting size of window
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);


    // Clearing backbuffer
    glViewport(0, 0, w, h);
    glDepthRange(0.00001, 10);
    const float clear_color[3] = { 0.3f, 0.3f, 0.8f };
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Fake projection matrix, scales with respect to window coordinates
    // PS: 1.f / w in [1][1] is correct.. do you know why ? (:
    float left = 0.f;// *-0.5;
    float top = 0.f;// (float)h * -0.5;
    float right = (float)w;// *0.5;
    float bottom = (float)h;// *0.5;

    float sx = 2.f / (right - left);
    float sy = 2.f / (top - bottom);
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    mat3 projection_2D{ { sx, 0.f, 0.f },{ 0.f, sy, 0.f },{ tx, ty, 1.f } };

   m_player.draw(projection_2D);
}

// Should the game be over ?
bool World::is_over() const {
   return glfwWindowShouldClose(m_window);
}

void World::on_key(GLFWwindow*, int key, int, int action, int mod){

    m_player.isMoveUp(false);
    m_player.isMoveDwn(false);
    m_player.isMoveLft(false);
    m_player.isMoveRht(false);

    //moving up and down
    if ((action == GLFW_REPEAT || action == GLFW_PRESS) && key == GLFW_KEY_W) {
        m_player.is_move();
        m_player.isMoveUp(true);
    }


    if ((action == GLFW_REPEAT || action == GLFW_PRESS) && key == GLFW_KEY_S) {
        m_player.is_move();
        m_player.isMoveDwn(true);
    }
    //
    //moving right and left
    if ((action == GLFW_REPEAT || action == GLFW_PRESS) && key == GLFW_KEY_D) {
        m_player.is_move();
        m_player.isMoveRht(true);
    }
    if ((action == GLFW_REPEAT || action == GLFW_PRESS) && key == GLFW_KEY_A) {
        m_player.is_move();
        m_player.isMoveLft(true);
    }



}


void World::on_mouse_move(GLFWwindow* window, double xpos, double ypos){

}
