// internal
#include "common.hpp"
#include "world.hpp"


// stlib
#include <chrono>
#include <iostream>

using Clock = std::chrono::high_resolution_clock;

// Global
World world;
const int screenWidth = 1200;
const int screenHeight = 800;

const int worldWidth = 5000;
<<<<<<< HEAD
const int worldHeight = 3000;
=======
const int worldHeight = 1000;
>>>>>>> master
const char* title = "DefendVancouver";

// Entry point
int main(int argc, char* argv[])
{
    // Initializing world (after renderer.init().. sorry)
    if (!world.init({(float) screenWidth, (float) screenHeight}, {(float) worldWidth, (float) worldHeight}))
    {
        // Time to read the error message
        std::cout << "Press any key to exit" << std::endl;
        std::cin.get();
        return EXIT_FAILURE;
    }

    auto t = Clock::now();

    // variable timestep loop.. can be improved (:
    while (!world.is_over())
    {
        // Processes system messages, if this wasn't present the window would become unresponsive
        glfwPollEvents();

        // Calculating elapsed times in milliseconds from the previous iteration
        auto now = Clock::now();
        float elapsed_sec = (float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
        t = now;

        world.update(elapsed_sec);
        world.draw();
    }

    world.destroy();

    return EXIT_SUCCESS;
}