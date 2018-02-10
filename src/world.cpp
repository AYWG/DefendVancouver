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
    const size_t MAX_BASENEMIES = 15;
    const size_t BENEMY_DELAY_MS = 2000;

	namespace
	{
		void glfw_err_cb(int error, const char* desc)
		{
			fprintf(stderr, "%d: %s", error, desc);
		}
	}
}

World::World() :
	m_points(0),
    m_next_benemy_spawn(0.f)
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


    m_current_speed = 1.f;
    m_is_advanced_mode = false;
    m_basEnemy.init();



	m_background.init();

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



	// faster based on current
	m_player.update(elapsed_ms);

    //basicEnemySpawning
    m_next_benemy_spawn -= elapsed_ms * m_current_speed;
    if(m_basEnemies.size() <= MAX_BASENEMIES && m_next_benemy_spawn){
        ////////////////////TODO////////////////
        if(!spawn_basicEnemy()){
            return false;
        }

        BasicEnemy& new_bEnemy = m_basEnemies.back();

        // Setting random initial position
        new_bEnemy.set_position({ screen.x + 150, 50 + m_dist(m_rng) * (screen.y - 100) });
        //new_bEnemy.set_position({ 50 + m_dist(m_rng) * (screen.x - 100), screen.y + 150  });
        // Next spawn
        m_next_benemy_spawn = (BENEMY_DELAY_MS / 2) + m_dist(m_rng) * (BENEMY_DELAY_MS / 2);
    }

    for (auto& bEnemy : m_basEnemies)
        bEnemy.update(elapsed_ms * m_current_speed);

    // Removing out of screen bEnemy
    auto benemy_it = m_basEnemies.begin();
    while (benemy_it != m_basEnemies.end())
    {
        float w = benemy_it->get_bounding_box().x / 2;
        if (benemy_it->get_position().x + w < 0.f)
        {
            benemy_it = m_basEnemies.erase(benemy_it);
            continue;
        }

        ++benemy_it;
    }


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
	glClearColor(clear_color[0], clear_color[0], clear_color[0], 1.0);
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

    m_background.draw(projection_2D);

	m_player.draw(projection_2D);
   // m_basEnemy.draw(projection_2D);

    for (auto& bEnemy : m_basEnemies)
        bEnemy.draw(projection_2D);




	// Presenting
	glfwSwapBuffers(m_window);
}

// Should the game be over ?
bool World::is_over()const
{
	return glfwWindowShouldClose(m_window);
}

// Creates a new turtle and if successfull adds it to the list of turtles
bool World::spawn_basicEnemy()
{
    BasicEnemy basicEnemy;
    if (basicEnemy.init())
    {
        m_basEnemies.emplace_back(basicEnemy);
        return true;
    }
    fprintf(stderr, "Failed to spawn turtle");
    return false;
}

// On key callback
void World::on_key(GLFWwindow*, int key, int, int action, int mod)
{


    if (key == GLFW_KEY_UP)
    {
        if (action == GLFW_PRESS)
        {
            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::UP);
            m_player.set_flying(true, Player::DIRECTION::UP);
        }
        else if (action == GLFW_RELEASE)
        {
            if (!m_is_advanced_mode)
            {
                m_player.set_velocity(0.f, Player::DIRECTION::UP);
            }
            m_player.set_flying(false, Player::DIRECTION::UP);
        }
    }

    if (key == GLFW_KEY_DOWN)
    {
        if (action == GLFW_PRESS)
        {
            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::DOWN);
            m_player.set_flying(true, Player::DIRECTION::DOWN);
        }
        else if (action == GLFW_RELEASE)
        {
            if (!m_is_advanced_mode)
            {
                m_player.set_velocity(0.f, Player::DIRECTION::DOWN);
            }
            m_player.set_flying(false, Player::DIRECTION::DOWN);
        }
    }

    if (key == GLFW_KEY_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::LEFT);
            m_player.set_flying(true, Player::DIRECTION::LEFT);
        }
        else if (action == GLFW_RELEASE)
        {
            if (!m_is_advanced_mode)
            {
                m_player.set_velocity(0.f, Player::DIRECTION::LEFT);
            }
            m_player.set_flying(false, Player::DIRECTION::LEFT);
        }
    }

    if (key == GLFW_KEY_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::RIGHT);
            m_player.set_flying(true, Player::DIRECTION::RIGHT);
        }
        else if (action == GLFW_RELEASE)
        {
            if (!m_is_advanced_mode)
            {
                m_player.set_velocity(0.f, Player::DIRECTION::RIGHT);
            }
            m_player.set_flying(false, Player::DIRECTION::RIGHT);
        }
    }



	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);

		m_player.init();

		m_current_speed = 1.f;

        if (key == GLFW_KEY_A)
        {
            m_is_advanced_mode = true;
        }
        if (key == GLFW_KEY_B)
        {
            m_is_advanced_mode = false;
        }
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

    playerCenter = m_player.get_position();
    auto x_pos = static_cast<float>(xpos);
    auto y_pos = static_cast<float>(ypos);
    mousePos = {x_pos, y_pos};
    aimDir = {mousePos.x - playerCenter.x, mousePos.y - playerCenter.y};
    aimDirNorm = {static_cast<float>(aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))),
                  static_cast<float>(aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)))};

    float rotation_angle = (atan2(aimDirNorm.x, -aimDirNorm.y));
    m_player.set_rotation(rotation_angle);
    //m_player.set_rotation(atan2((xpos - m_player.get_position().x), -(ypos - m_player.get_position().y) ) );

}
