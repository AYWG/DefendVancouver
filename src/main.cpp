#include "world.hpp"

#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>








World world;

using namespace std;

int main() {


    /*if (!glfwInit()) {
        // Handle initialization failure
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow *window = glfwCreateWindow(1280, 720, "DefendVancouver", NULL, NULL);

    int w, h;

    glfwGetFramebufferSize(window, &w, &h);

    glViewport(0, 0, w, h);
    glDepthRange(0.00001, 10);
    const float clear_color[3] = {0.3f, 0.3f, 0.8f};
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
*/
    //player m_plyr;
    //m_plyr.init();

    world.init({1280,720});


   /* while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
*/

}