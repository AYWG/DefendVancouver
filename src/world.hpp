#pragma once

// internal
#include "common.hpp"
#include "player.hpp"
#include "basicEnemy.hpp"
#include "bomber.hpp"
#include "chaser.hpp"

// stlib
#include <vector>
#include <random>



// Container for all our entities and game logic. Individual rendering / update is 
// deferred to the relative update() methods
class World
{
public:
	World();
	~World();

	// Creates a window, sets up events and begins the game
	bool init(vec2 screen);

	// Releases all associated resources
	void destroy();

	// Steps the game ahead by ms milliseconds
	bool update(float ms);

	// Renders our scene
	void draw();

	// Should the game be over ?
	bool is_over()const;

    vec2 playerCenter;
    vec2 mousePos;
    vec2 aimDir;
    vec2 aimDirNorm;

private:


	// !!! INPUT CALLBACK FUNCTIONS
	void on_key(GLFWwindow*, int key, int, int action, int mod);
	void on_mouse_move(GLFWwindow* window, double xpos, double ypos);

private:
	// Window hjandle
	GLFWwindow* m_window;

	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int m_points;

	// Game entities
    
	Player m_player;

	float m_current_speed;
	bool m_is_advanced_mode;

	// C++ rng
	std::default_random_engine m_rng;
	std::uniform_real_distribution<float> m_dist; // default 0..1
};
