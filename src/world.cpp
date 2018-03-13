// Header
#include "world.hpp"



// stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>

typedef pair<int, int> Pair;

// Same as static in c, local to compilation unit
namespace {

    const size_t MAX_BOMBS = 5;
    const size_t BULLET_DELAY_MS = 200;
    const size_t MAX_SHOOTERS = 15;
    const size_t MAX_CHASER = 0;
    const size_t SHOOTER_DELAY_MS = 2000;
    const size_t BOMB_DELAY_MS = 5000;


    namespace {
        void glfw_err_cb(int error, const char *desc) {
            fprintf(stderr, "%d: %s", error, desc);
        }
    }
}

World::World() :
	m_points(0),
    m_next_shooter_spawn(0.f),
    m_next_chaser_spawn(0.f),
    m_next_bomb_spawn(0.f)

{
	// Seeding rng with random device
	m_rng = std::default_random_engine(std::random_device()());
}

World::~World() {

}

// World initialization
bool World::init(vec2 screenSize, vec2 worldSize) {
    //-------------------------------------------------------------------------
    // GLFW / OGL Initialization
    // Core Opengl 3.
    glfwSetErrorCallback(glfw_err_cb);

    if (!glfwInit()) {
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
    m_window = glfwCreateWindow((int) screenSize.x, (int) screenSize.y, "DefendVancouver", nullptr, nullptr);
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
    auto mouse_button_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2) {
        ((World *) glfwGetWindowUserPointer(wnd))->onMouseClick(wnd, _0, _1, _2); };
	glfwSetKeyCallback(m_window, key_redirect);
	glfwSetCursorPosCallback(m_window, cursor_pos_redirect);
    glfwSetMouseButtonCallback(m_window, mouse_button_redirect);

    m_size = worldSize;
    m_camera.setSize(screenSize);

    m_background.init();

    m_camera.setFocusPoint(m_player.getPosition());
	return m_player.init();

}

// Releases all the associated resources
void World::destroy() {


    glfwDestroyWindow(m_window);
}

// Update our game world
bool World::update(float elapsed_ms) {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    vec2 screen = {(float) w, (float) h};

    m_player.update(elapsed_ms);
    vec2 playerPos = m_player.getPosition();

    // update camera
    auto newCameraFocusPointX = std::min(m_size.x - screen.x/2, std::max(screen.x / 2, playerPos.x));
    auto newCameraFocusPointY = std::min(m_size.y - screen.y/2, std::max(screen.y / 2, playerPos.y));
    m_camera.setFocusPoint({newCameraFocusPointX, newCameraFocusPointY});

    for (auto &playerBullet : m_player.getBullets()){
        playerBullet->update(elapsed_ms);
    }

    // remove out of screen player bullets
    auto playerBulletIt = m_player.getBullets().begin();
    while (playerBulletIt != m_player.getBullets().end()) {
        if ((*playerBulletIt)->getPosition().y >  m_camera.getBottomBoundary() ||
            (*playerBulletIt)->getPosition().y  <  m_camera.getTopBoundary() ||
            (*playerBulletIt)->getPosition().x > m_camera.getRightBoundary() ||
            (*playerBulletIt)->getPosition().x < m_camera.getLeftBoundary()) {
            playerBulletIt = m_player.getBullets().erase(playerBulletIt);
            continue;
        }
        ++playerBulletIt;
    }

    m_next_shooter_spawn -= elapsed_ms;
    if (m_shooters.size() <= MAX_SHOOTERS && m_next_shooter_spawn) {
        if (!spawnShooter()) {
            return false;
        }
        Shooter& shooter = m_shooters.back();

        // Setting random initial position
        shooter.setPosition({ 50 + m_dist(m_rng) * screen.x, -200.f  });
        // Next spawn
        m_next_shooter_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
    }

    for (auto &shooter : m_shooters) {
        shooter.update(this, elapsed_ms);
        for (auto &shooterBullet : shooter.getBullets()) {
            shooterBullet->update(elapsed_ms);
        }

        // remove out of screen shooter bullets - remove once we have proper collisions
        auto shooterBulletIt = shooter.getBullets().begin();
        while (shooterBulletIt != shooter.getBullets().end()) {
            if ((*shooterBulletIt)->getPosition().y > 1000) {
                shooterBulletIt = shooter.getBullets().erase(shooterBulletIt);
                continue;
            }
            ++shooterBulletIt;
        }
    }
/*
    auto benemy_col = m_shooters.begin();
    auto pbullet_col = m_bullets.begin();
    float boundBullet = pbullet_col->getBoundingBox().x/2;
    float boundEnemy = benemy_col->getBoundingBox().x / 2;
    while (pbullet_col != m_bullets.end() && benemy_col != m_shooters.end()) {
        if (pbullet_col->getPosition().y + boundBullet <
                benemy_col->getPosition().y + boundEnemy){
            std::cout<<"hit";
            benemy_col = m_shooters.erase(benemy_col);
            pbullet_col = m_bullets.erase(pbullet_col);
*/

//////////////////CHASER///////////////////
    m_next_chaser_spawn -= elapsed_ms;
    if (m_chasers.size() <= MAX_CHASER && m_next_chaser_spawn) {

        ////////////////////TODO////////////////
        if (!spawnChaser()) {
            return false;
        }

        Chaser &new_cEnemy = m_chasers.back();

        // Setting random initial position
        // new_bEnemy.setPosition(({ screen.x + 150, 50 + m_dist(m_rng) * (screen.y - 100) });
        new_cEnemy.setPosition({50 + m_dist(m_rng) * (screen.x), screen.y - 800});
        // Next spawn
        m_next_chaser_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
    }

    for (auto &m_chaser : m_chasers)
        m_chaser.update(this, elapsed_ms);

    //////////////SPAWNDONE/////////////////
    //ASTAR
    int j = 0;
    int l = 0;

    int grid[ROW][COL];

    for (int i = 0; i <= 99; i++) {
        for (int j = 0; j <= 99; j++) {
            grid[i][j] = 1;
        }
    }

    bool srcFound = false;
    bool destFound = false;

    //width -> -600 to 2060
    //height -> -150 to 1000

    float width = 26.6f;
    float height = 11.5f;

    for (auto &m_chaser : m_chasers) {
        if (!srcFound) {
            for (float k = -600.f/*0.f*/; k <= 2060 /*1200.f*/; k += width) {
                for (float i = /*0*/-150.f; i <= 1000.f; i += height) {
                    if (m_chaser.getPosition().y >= 0.f && m_chaser.getPosition().y < height
                        && m_chaser.getPosition().x >= 0.f && m_chaser.getPosition().x < width) {
                        //  Pair src = make_pair(0, 0);
                        srcFound = true;
                        if (srcFound) {
                            break;
                        }

                    } else if ((m_chaser.getPosition().y >= (i) && m_chaser.getPosition().y < (i + height))
                               && (m_chaser.getPosition().x >= (k) && m_chaser.getPosition().x < (k + width))) {
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
            for (float k = /*0*/-600.f; k <= /*1200*/2060.f; k += width) {
                for (float i = -150.f; i <= 1000.f; i += height) {
                    if (m_player.getPosition().y >= 0.f && m_player.getPosition().y < height
                        && m_player.getPosition().x >= 0.f && m_player.getPosition().x < width) {
                        //Pair dest = make_pair(0, 0);
                        destFound = true;
                        if (destFound) {
                            break;
                        }
                    } else if ((m_player.getPosition().y >= (i) && m_player.getPosition().y < (i + height))
                               && (m_player.getPosition().x >= (k) && m_player.getPosition().x < (k + width))) {
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


        m_chaser.update(this, elapsed_ms);

        if (destFound && srcFound) {
            Pair src = make_pair(j, l);
            Pair dest = make_pair(a, b);
            m_chaser.aStarSearch(grid, src, dest);
        }
    }


    // trigger bomb animation
    for (auto& bomb : m_bombs)
        bomb.update(elapsed_ms);

    // removing bombs from screen
    auto bomb_it = m_bombs.begin();
    while (bomb_it != m_bombs.end())
    {
        int fc = bomb_it->getFrameCount();
        if (fc == 0)
        {
            bomb_it = m_bombs.erase(bomb_it);
            continue;
        }

        ++bomb_it;
    }

    // Spawn new regular bombs
    m_next_bomb_spawn -= elapsed_ms;
    if (m_bombs.size() <= MAX_BOMBS && m_next_bomb_spawn < 0.f)
    {
        if (!spawn_bomb())
            return false;
        Bomb& new_bomb = m_bombs.back();

        //new_bomb.set_position({ screen.x + 150, 50 + m_dist(m_rng) *  (screen.y - 100) });
        new_bomb.set_position(getPlayerPosition());

        m_next_bomb_spawn = (BOMB_DELAY_MS / 2) + m_dist(m_rng) * (BOMB_DELAY_MS / 2);
    }

    return true;
}

bool World::elapsedUpdate(float elapsed_ms) {

    return true;

}


// Render our game world
void World::draw() {
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
    const float clear_color[3] = {0.04f, 0.02f, 0.11f};
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
    mat3 projection_2D{{sx,  0.f, 0.f},
                       {0.f, sy,  0.f},
                       {tx,  ty,  1.f}};

    // Drawing entities
    m_background.draw(projection_2D);

	m_player.draw(projection_2D);

    for (auto &m_chaser : m_chasers)
        m_chaser.draw(projection_2D);

    for (auto &shooter : m_shooters) {
        shooter.draw(projection_2D);
        for (auto &bullet : shooter.getBullets()) {
            bullet->draw(projection_2D);
        }
    }


   for (auto &bullet : m_player.getBullets()) {
        bullet->draw(projection_2D);
   }


    for (auto& bomb : m_bombs){
        bomb.draw(projection_2D);
    }

    // Presenting
    glfwSwapBuffers(m_window);
}

// Should the game be over ?
bool World::is_over() const {
    return glfwWindowShouldClose(m_window);
}

vec2 World::getPlayerPosition() const {
    return m_player.getPosition();
}

std::vector<vec2> World::getBombPositions() const {
    auto positions = std::vector<vec2>();
    for (auto& bomb : m_bombs) {
        positions.emplace_back(bomb.get_position());
    }
    return positions;
}

vec2 World::getCityPosition() const {
    return m_background.getPosition();
}

bool World::spawnShooter() {
    ShooterAI ai;
    Shooter shooter(ai);
    if (shooter.init()) {
        m_shooters.emplace_back(shooter);
        return true;
    }
    fprintf(stderr, "Failed to spawn shooter");
    return false;
}

bool World::spawnChaser() {
    ChaserAI ai;
    Chaser chaser(ai);
    if (chaser.init()) {
        m_chasers.emplace_back(chaser);
        return true;
    }
    return false;
}

bool World::spawn_bomb()
{
    Bomb bomb;
    if (bomb.init(textures_path("normal_bomb.png")))
    {
        m_bombs.emplace_back(bomb);
        return true;
    }
    fprintf(stderr, "Failed to spawn bomb");
    return false;
}

// On key callback
void World::onKey(GLFWwindow *, int key, int, int action, int mod) {
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
//            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::UP);
            m_player.setFlying(Player::DIRECTION::FORWARD, true);
        } else if (action == GLFW_RELEASE) {
            m_player.setFlying(Player::DIRECTION::FORWARD, false);
        }
    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
//            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::DOWN);
            m_player.setFlying(Player::DIRECTION::BACKWARD, true);
        } else if (action == GLFW_RELEASE) {
            m_player.setFlying(Player::DIRECTION::BACKWARD, false);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
//            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::LEFT);
            m_player.setFlying(Player::DIRECTION::LEFT, true);
        } else if (action == GLFW_RELEASE) {
            m_player.setFlying(Player::DIRECTION::LEFT, false);
        }
    }

    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
//            m_player.set_velocity(m_player.get_max_speed(), Player::DIRECTION::RIGHT);
            m_player.setFlying(Player::DIRECTION::RIGHT, true);
        } else if (action == GLFW_RELEASE) {
            m_player.setFlying(Player::DIRECTION::RIGHT, false);
        }
    }

    // Resetting game
    if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
        int w, h;
        glfwGetWindowSize(m_window, &w, &h);

        m_player.init();

    }
}


void World::onMouseMove(GLFWwindow *window, double xpos, double ypos) {

    playerCenter = {m_player.getPosition().x - m_camera.getLeftBoundary(),
                    m_player.getPosition().y - m_camera.getTopBoundary()};
    auto playerMouseXDist = float(xpos - playerCenter.x);
    auto playerMouseYDist = float(ypos - playerCenter.y);
    float newOrientation = -1.f * atan((playerMouseXDist / playerMouseYDist));
    if (playerMouseYDist < 0.f) newOrientation += 3.1415f;
    m_player.setRotation(newOrientation);
}

void World::onMouseClick(GLFWwindow *window, int button, int action, int mod) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            m_player.enableShooting(true);
        } else if (action == GLFW_RELEASE) {
            m_player.enableShooting(false);
        }
    }
}