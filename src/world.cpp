// Header
#include "world.hpp"
#include <bits/stdc++.h>


// stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>

typedef  pair<int, int> Pair;

// Same as static in c, local to compilation unit
namespace
{

    const size_t MAX_BASENEMIES = 1;
    const size_t MAX_BULLET = 1;
    const size_t BENEMY_DELAY_MS = 2000;
    const size_t PBULLET_DELAY_MS = 2000;
    const size_t MAX_SHOOTERS = 15;
    const size_t SHOOTER_DELAY_MS = 2000;


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
    m_next_pbullet_spawn(0.f),
    m_next_shooter_spawn(0.f),
    m_next_chaser_spawn(0.f)

{
	// Seeding rng with random device
	m_rng = std::default_random_engine(std::random_device()());
}

World::~World()
{

}

// World initialization
bool World::init(vec2 screenSize, vec2 worldSize)
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
	m_window = glfwCreateWindow((int)screenSize.x, (int)screenSize.y, "DefendVancouver", nullptr, nullptr);
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
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) {
        ((World *) glfwGetWindowUserPointer(wnd))->onKey(wnd, _0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) {
        ((World *) glfwGetWindowUserPointer(wnd))->onMouseMove(wnd, _0, _1); };
	glfwSetKeyCallback(m_window, key_redirect);
	glfwSetCursorPosCallback(m_window, cursor_pos_redirect);

    m_size = worldSize;
    m_camera.setSize(screenSize);

    m_current_speed = 1.f;
    m_is_advanced_mode = false;

  //  m_chaser.init();

    m_player.init();

    m_plbullet.init();
    //m_bomber.init();
    m_background.init();

	//m_background.init();
    m_camera.setFocusPoint(m_player.get_position());
	return m_player.init();
	//return true;
}

// Releases all the associated resources
void World::destroy()
{


	glfwDestroyWindow(m_window);
}

// Update our game world
bool World::update(float elapsed_ms) {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    vec2 screen = {(float) w, (float) h};



    // faster based on current
    m_player.update(elapsed_ms);

    vec2 playerPos = m_player.get_position();
    // update camera
    if (((playerPos.x - screen.x / 5 >= 0) && (playerPos.x - 6 * screen.x / 5 <= 0)) &&
        playerPos.x + screen.x / 2 <= m_size.x) {
        m_camera.setFocusPoint({playerPos.x, m_camera.getFocusPoint().y});
    }
    if (playerPos.y - screen.y / 3 >= 0 && playerPos.y - 3 * screen.y / 4 <= 0 &&
        playerPos.y + screen.y / 2 <= m_size.y) {
        m_camera.setFocusPoint({m_camera.getFocusPoint().x, playerPos.y});
    }


    //bullet
    m_next_pbullet_spawn -= elapsed_ms * m_plbullet.m_velocity;
    // if(m_pbullet.size() <= MAX_BASENEMIES && m_next_pbullet_spawn) {
    if (!spawn_playerBullet()) {
        return false;
    }
    if (is_shot) {
        pBullet &new_pBullet = m_pbullet.back();
        new_pBullet.setPosition(m_player.get_position());
    }
    for (auto& pBullet : m_pbullet){
        pBullet.update(elapsed_ms * m_plbullet.m_velocity);

        if (is_shot) {
           // pBullet = m_pbullet.back();
           // pBullet.setPosition()(m_player.getPosition());
            pBullet.fireBullet({pBullet.m_velocity * mouseAimDir.x , pBullet.m_velocity * mouseAimDir.y });
            afterShot = {pBullet.m_velocity * mousePosition().x, pBullet.m_velocity * mousePosition().y};

        } else {
            pBullet.update(elapsed_ms * m_plbullet.m_velocity);
            pBullet.fireBullet({afterShot.x, afterShot.y});


        }

    }


    auto pbullet_it = m_pbullet.begin();

    while (pbullet_it != m_pbullet.end()) {
        if (pbullet_it->getPosition().y >  m_camera.getBottomBoundary() ||
            pbullet_it->getPosition().y  <  m_camera.getTopBoundary() ||
            pbullet_it->getPosition().x > m_camera.getRightBoundary() ||
            pbullet_it->getPosition().x < m_camera.getLeftBoundary()) {
            // m_pbullet.pop_back();
            pbullet_it = m_pbullet.erase(pbullet_it);
            continue;
        }

        ++pbullet_it;
    }


    //basicEnemySpawning
    m_next_shooter_spawn -= elapsed_ms * m_current_speed;
    if(m_shooters.size() <= MAX_SHOOTERS && m_next_shooter_spawn){

        ////////////////////TODO////////////////
        if(!spawnShooter()){
            return false;
        }

        Shooter& new_bEnemy = m_shooters.back();

        // Setting random initial position
       // new_bEnemy.setPosition(({ screen.x + 150, 50 + m_dist(m_rng) * (screen.y - 100) });
        new_bEnemy.setPosition({ 50 + m_dist(m_rng) * (screen.x), screen.y - 800  });
        // Next spawn
        m_next_shooter_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
    }

    for (auto& bEnemy : m_shooters)
        bEnemy.update(this, elapsed_ms * m_current_speed);

    // Removing out of screen bEnemy
    auto benemy_it = m_shooters.begin();
    while (benemy_it != m_shooters.end())
    {
        float w = benemy_it->getBoundingBox().x / 2;
        if (benemy_it->getPosition().y + w > screen.y)
        {
            benemy_it = m_shooters.erase(benemy_it);
            continue;
        }

        ++benemy_it;
    }



/*
    auto benemy_col = m_shooters.begin();
    auto pbullet_col = m_pbullet.begin();
    float boundBullet = pbullet_col->getBoundingBox().x/2;
    float boundEnemy = benemy_col->getBoundingBox().x / 2;
    while (pbullet_col != m_pbullet.end() && benemy_col != m_shooters.end()) {
        if (pbullet_col->getPosition().y + boundBullet <
                benemy_col->getPosition().y + boundEnemy){
            std::cout<<"hit";
            benemy_col = m_shooters.erase(benemy_col);
            pbullet_col = m_pbullet.erase(pbullet_col);
*/

//////////////////CHASER///////////////////
    //basicEnemySpawning
    m_next_chaser_spawn -= elapsed_ms * m_current_speed;
    if(m_chasers.size() <= MAX_SHOOTERS && m_next_chaser_spawn){

        ////////////////////TODO////////////////
        if(!spawnChaser()){
            return false;
        }

        Chaser& new_cEnemy = m_chasers.back();

        // Setting random initial position
        // new_bEnemy.setPosition(({ screen.x + 150, 50 + m_dist(m_rng) * (screen.y - 100) });
        new_cEnemy.setPosition({ 50 + m_dist(m_rng) * (screen.x), screen.y - 800  });
        // Next spawn
        m_next_chaser_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
    }

    for (auto& m_chaser : m_chasers)
        m_chaser.update(this, elapsed_ms * m_current_speed);

    // Removing out of screen cEnemy
    auto cenemy_it = m_chasers.begin();
    while (cenemy_it != m_chasers.end())
    {
        float w = cenemy_it->getBoundingBox().x / 2;
        if (cenemy_it->getPosition().y + w > screen.y)
        {
            cenemy_it = m_chasers.erase(cenemy_it);
            continue;
        }

        ++cenemy_it;
    }
    //////////////SPAWNDONE/////////////////
    //ASTAR
    int j = 0;
    int l = 0;

    int grid[ROW][COL];

    for (int i = 0; i <= 9; i++){
        for (int j = 0; j <= 9; j++){
            grid[i][j] = 1;
        }
    }


    bool srcFound = false;
    bool destFound = false;

    for (auto& m_chaser : m_chasers) {
        if (!srcFound) {
            for (float k = -600.f/*0.f*/; k <= 2060 /*1200.f*/; k += 266) {
                for (float i = /*0*/-150.f; i <= 1000.f; i += 115.f) {
                    if (m_chaser.getPosition().y >= 0.f && m_chaser.getPosition().y < 115.f
                        && m_chaser.getPosition().x >= 0.f && m_chaser.getPosition().x < 266.f) {
                        //  Pair src = make_pair(0, 0);
                        srcFound = true;
                        if (srcFound) {
                            break;
                        }

                    } else if ((m_chaser.getPosition().y >= (i) && m_chaser.getPosition().y < (i + 115.f))
                               && (m_chaser.getPosition().x >= (k) && m_chaser.getPosition().x < (k + 266.f))) {
                        // Pair src = make_pair(j,l);
                        srcFound = true;
                        if (srcFound) {
                            break;
                        }
                    }
                    j++;
                }
                if (srcFound) {
                    break;
                }
                l++;
                j = 0;
            }
        }

        int a = 0;
        int b = 0;

        if (!destFound) {
            for (float k = /*0*/-600.f; k <= /*1200*/2060.f; k += 266) {
                for (float i = -150.f; i <= 1000.f; i += 115.f) {
                    if (m_player.get_position().y >= 0.f && m_player.get_position().y < 115.f
                        && m_player.get_position().x >= 0.f && m_player.get_position().x < 266.f) {
                        //Pair dest = make_pair(0, 0);
                        destFound = true;
                        if (destFound) {
                            break;
                        }
                    } else if ((m_player.get_position().y >= (i) && m_player.get_position().y < (i + 115.f))
                               && (m_player.get_position().x >= (k) && m_player.get_position().x < (k + 266.f))) {
                        //Pair dest = make_pair(a,b);
                        destFound = true;
                        if (destFound) {
                            break;
                        }
                    }

                    a++;
                }
                if (destFound) {
                    break;
                }
                b++;
                a = 0;
            }
        }
/*    int r = ceil(m_bomber.get_position().x/266);
    int s = ceil(m_bomber.get_position().y/115);

    std::cout<<s<<", "<<r<<std::endl;

    grid[s][r] = 0;*/



        m_chaser.update(this, elapsed_ms);

        if (destFound && srcFound) {
            Pair src = make_pair(j, l);
            Pair dest = make_pair(a, b);
            m_chaser.aStarSearch(grid, src, dest);
        }
    }




    return true;
}

bool World::elapsedUpdate(float elapsed_ms) {

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
	const float clear_color[3] = { 0.04f, 0.02f, 0.11f };
	glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Fake projection matrix, scales with respect to window coordinates
	// PS: 1.f / w in [1][1] is correct.. do you know why ? (:
	float left = m_camera.getLeftBoundary();
    float top = m_camera.getTopBoundary();
	float right = m_camera.getRightBoundary();
	float bottom = m_camera.getBottomBoundary();

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	mat3 projection_2D{ { sx, 0.f, 0.f },{ 0.f, sy, 0.f },{ tx, ty, 1.f } };

	// Drawing entities

    m_background.draw(projection_2D);

 //   m_plbullet.draw(projection_2D);

	m_player.draw(projection_2D);

    //m_bomber.draw(projection_2D);

   // m_shooter.draw(projection_2D);

    for (auto& m_chaser : m_chasers)
        m_chaser.draw(projection_2D);

    for (auto& shooter : m_shooters)
        shooter.draw(projection_2D);


       for (auto &bBullet : m_pbullet) {
          //  if(is_shot) {
                bBullet.draw(projection_2D);
          //  }
           }

    for (auto &shotBullet : m_shotBullet) {
        //  if(is_shot) {
        shotBullet.draw(projection_2D);
        //  }
    }


	// Presenting
	glfwSwapBuffers(m_window);
}

// Should the game be over ?
bool World::is_over()const
{
	return glfwWindowShouldClose(m_window);
}

vec2 World::getPlayerPosition() const {
    return m_player.get_position();
}


vec2 const  World::mousePosition(){
    return get_mousePos(aimDirNorm);
}

vec2 World::get_mousePos(vec2 mousePos) {
    mousePos = mouseAimDir;
    return  mousePos;
}


bool World::spawnShooter() {
    ShooterAI ai;
    Shooter shooter(ai);
    if (shooter.init())
    {
        m_shooters.emplace_back(shooter);
        return true;
    }
    fprintf(stderr, "Failed to spawn shooter");
    return false;
}

bool World::spawnChaser() {
    ChaserAI ai;
    Chaser chaser(ai);
    if (chaser.init())
    {
        m_chasers.emplace_back(chaser);
        return true;
    }
    return false;
}

bool World::spawn_playerBullet()
{
    pBullet playerBullet;
    if (playerBullet.init())
    {
        m_pbullet.emplace_back(playerBullet);
        return true;
    }
    fprintf(stderr, "Failed to spawn turtle");
    return false;
}

// On key callback
void World::onKey(GLFWwindow *, int key, int, int action, int mod) {


    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::UP);
            m_player.set_flying(true, Player::DIRECTION::UP);
        } else if (action == GLFW_RELEASE) {
            if (!m_is_advanced_mode) {
                m_player.set_velocity(0.f, Player::DIRECTION::UP);
            }
            m_player.set_flying(false, Player::DIRECTION::UP);
        }
    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::DOWN);
            m_player.set_flying(true, Player::DIRECTION::DOWN);
        } else if (action == GLFW_RELEASE) {
            if (!m_is_advanced_mode) {
                m_player.set_velocity(0.f, Player::DIRECTION::DOWN);
            }
            m_player.set_flying(false, Player::DIRECTION::DOWN);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::LEFT);
            m_player.set_flying(true, Player::DIRECTION::LEFT);
        } else if (action == GLFW_RELEASE) {
            if (!m_is_advanced_mode) {
                m_player.set_velocity(0.f, Player::DIRECTION::LEFT);
            }
            m_player.set_flying(false, Player::DIRECTION::LEFT);
        }
    }

    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::RIGHT);
            m_player.set_flying(true, Player::DIRECTION::RIGHT);
        } else if (action == GLFW_RELEASE) {
            if (!m_is_advanced_mode) {
                m_player.set_velocity(0.f, Player::DIRECTION::RIGHT);
            }
            m_player.set_flying(false, Player::DIRECTION::RIGHT);
        }
    }

    //is_shot = false;

    //SHOOTING
 /*   if (key == GLFW_KEY_SPACE){
        if (action == GLFW_PRESS){
            is_shot = true;
        }else if (action == GLFW_RELEASE){
            is_shot = false;
        }*/
    is_shot = false;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        is_shot = true;
        std::cout<<"the y value: "<< m_player.get_position().y<<std::endl;

    }




        // Resetting game
        if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
            int w, h;
            glfwGetWindowSize(m_window, &w, &h);

            m_player.init();

            m_current_speed = 1.f;

            if (key == GLFW_KEY_A) {
                m_is_advanced_mode = true;
            }
            if (key == GLFW_KEY_B) {
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


void World::onMouseMove(GLFWwindow *window, double xpos, double ypos)
{

    playerCenter = { m_player.get_position().x - m_camera.getLeftBoundary(), m_player.get_position().y - m_camera.getTopBoundary() };


    auto x_pos = static_cast<float>(xpos);
    auto y_pos = static_cast<float>(ypos);
    mousePos = {x_pos, y_pos};
    aimDir = {mousePos.x - playerCenter.x, mousePos.y - playerCenter.y};

    aimDirNorm = {static_cast<float>(aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))),
                   static_cast<float>(aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)))};


    float rotation_angle = (atan2(aimDirNorm.x, -aimDirNorm.y));

    //mouseAimDir =   {aimDir.x, aimDir.y} ;
    mouseAimDir = aimDirNorm;

    m_player.set_rotation(rotation_angle);
    //m_player.set_rotation(atan2((xpos - m_player.getPosition().x), -(ypos - m_player.getPosition().y) ) );
}
