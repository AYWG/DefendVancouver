// Header
#include "world.hpp"

// stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>


// Same as static in c, local to compilation unit
namespace
{

	namespace
	{
		void glfw_err_cb(int error, const char* desc)
		{
			fprintf(stderr, "%d: %s", error, desc);
		}
	}
}

World::World() :
	m_points(0)
{
	// Seeding rng with random device
	m_rng = std::default_random_engine(std::random_device()());
}

World::~World()
{

}

// World initialization
bool World::init(vec2 screen)
{
	//-------------------------------------------------------------------------
	// GLFW / OGL Initialization
	// Core Opengl 3.
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);
	m_window = glfwCreateWindow((int)screen.x, (int)screen.y, "DefendVancouver", nullptr, nullptr);
	if (m_window == nullptr)
		return false;

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1); // vsync

						 // Load OpenGL function pointers
	gl3wInit();

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(m_window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((World*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
	glfwSetKeyCallback(m_window, key_redirect);
	glfwSetCursorPosCallback(m_window, cursor_pos_redirect);

	return m_player.init();
	//return true;
}

// Releases all the associated resources
void World::destroy()
{


	glfwDestroyWindow(m_window);
}

// Update our game world
bool World::update(float elapsed_ms)
{
	int w, h;
	glfwGetFramebufferSize(m_window, &w, &h);
	vec2 screen = { (float)w, (float)h };



	// Updating all entities, making the turtle and fish
	// faster based on current
	m_player.update(elapsed_ms);


	return true;
}

// Render our game world
void World::draw()
{
	// Clearing error buffer
	gl_flush_errors();

	// Getting size of window
	int w, h;
	glfwGetFramebufferSize(m_window, &w, &h);


	// Updating window title with points
	std::stringstream title_ss;
	title_ss << "Points: " << m_points;
	glfwSetWindowTitle(m_window, title_ss.str().c_str());

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

	// Drawing entities

	m_player.draw(projection_2D);

	// Presenting
	glfwSwapBuffers(m_window);
}

// Should the game be over ?
bool World::is_over()const
{
	return glfwWindowShouldClose(m_window);
}



// On key callback
void World::on_key(GLFWwindow*, int key, int, int action, int mod)
{

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



	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);

		m_player.init();

		m_current_speed = 1.f;
	}

	// Control the current speed with `<` `>`
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_COMMA)
		m_current_speed -= 0.1f;
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD)
		m_current_speed += 0.1f;

	m_current_speed = fmax(0.f, m_current_speed);
}

void World::on_mouse_move(GLFWwindow* window, double xpos, double ypos)
{

    vec2 playerCenter = m_player.get_position();
    vec2 mousePos = {xpos, ypos};
    vec2 aimDir = {mousePos.x - playerCenter.x, mousePos.y - playerCenter.y};
    vec2 aimDirNorm = {aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)),
                       aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))};

    m_player.set_rotation((atan2(aimDirNorm.x, -aimDirNorm.y)));
    //m_player.set_rotation(atan2((xpos - m_player.get_position().x), -(ypos - m_player.get_position().y) ) );




}
