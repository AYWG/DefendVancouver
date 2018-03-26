// Header
#include "world.hpp"



// stlib
#include <sstream>
#include <iostream>
#include <math.h>

typedef pair<int, int> Pair;

// Same as static in c, local to compilation unit
namespace {

    const size_t MAX_BOMBS = 0;
    const size_t MAX_BOMBERBOMBS = 0;
    const size_t MAX_SHOOTERS = 15;
    const size_t MAX_CHASER = 3;
    const size_t MAX_BOMBER = 1;
    const size_t SHOOTER_DELAY_MS = 2000;
    const size_t BOMB_DELAY_MS = 2000;


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
        m_next_bomber_spawn(0.f),
        m_next_nbomb_spawn(0.f),
        m_next_bbomb_spawn(0.f) {
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
    auto key_redirect = [](GLFWwindow *wnd, int _0, int _1, int _2, int _3) {
        ((World *) glfwGetWindowUserPointer(wnd))->onKey(wnd, _0, _1, _2, _3);
    };
    auto cursor_pos_redirect = [](GLFWwindow *wnd, double _0, double _1) {
        ((World *) glfwGetWindowUserPointer(wnd))->onMouseMove(wnd, _0, _1);
    };
    auto mouse_button_redirect = [](GLFWwindow *wnd, int _0, int _1, int _2) {
        ((World *) glfwGetWindowUserPointer(wnd))->onMouseClick(wnd, _0, _1, _2);
    };
    glfwSetKeyCallback(m_window, key_redirect);
    glfwSetCursorPosCallback(m_window, cursor_pos_redirect);
    glfwSetMouseButtonCallback(m_window, mouse_button_redirect);

    m_size = worldSize;
    m_camera.setSize(screenSize);
    initTextures();
    m_background.init();
    m_camera.setFocusPoint(m_player.getPosition());
    return m_player.init(worldSize);

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
    auto newCameraFocusPointX = std::min(m_size.x - screen.x / 2, std::max(screen.x / 2, playerPos.x));
    auto newCameraFocusPointY = std::min(m_size.y - screen.y / 2, std::max(screen.y / 2, playerPos.y));
    m_camera.setFocusPoint({newCameraFocusPointX, newCameraFocusPointY});

    for (auto &playerBullet : m_player.getBullets()) {
        playerBullet->update(elapsed_ms);
    }

    // remove out of screen player bullets
    auto playerBulletIt = m_player.getBullets().begin();
    while (playerBulletIt != m_player.getBullets().end()) {
        if ((*playerBulletIt)->getPosition().y > m_camera.getBottomBoundary() ||
            (*playerBulletIt)->getPosition().y < m_camera.getTopBoundary() ||
            (*playerBulletIt)->getPosition().x > m_camera.getRightBoundary() ||
            (*playerBulletIt)->getPosition().x < m_camera.getLeftBoundary()) {
            playerBulletIt = m_player.getBullets().erase(playerBulletIt);
            continue;
        }
        ++playerBulletIt;
    }

    m_next_shooter_spawn -= elapsed_ms;
    if (m_shooters.size() < MAX_SHOOTERS && m_next_shooter_spawn) {
        if (auto newShooter = Shooter::spawn()) {
            m_shooters.emplace_back(newShooter);
        }
        auto newShooterPtr = m_shooters.back();

        // Setting random initial position
        newShooterPtr->setPosition({m_dist(m_rng) * m_size.x, -200.f});
        // Next spawn
        m_next_shooter_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
    }

    for (auto &shooter : m_shooters) {
        shooter->update(this, elapsed_ms);
        for (auto &shooterBullet : shooter->getBullets()) {
            shooterBullet->update(elapsed_ms);
        }

        // remove out of screen shooter bullets - remove once we have proper collisions
        auto shooterBulletIt = shooter->getBullets().begin();
        while (shooterBulletIt != shooter->getBullets().end()) {
            if ((*shooterBulletIt)->getPosition().y > m_size.y) {
                shooterBulletIt = shooter->getBullets().erase(shooterBulletIt);
                continue;
            }
            ++shooterBulletIt;
        }
    }

//////////////////CHASER///////////////////
    m_next_chaser_spawn -= elapsed_ms;
    if (m_chasers.size() < MAX_CHASER && m_next_chaser_spawn) {

        ////////////////////TODO////////////////
        if (auto newChaser = Chaser::spawn()) {
            m_chasers.emplace_back(newChaser);
        }
        auto newChaserPtr = m_chasers.back();

        // Setting random initial position
        newChaserPtr->setPosition({50 + m_dist(m_rng) * (screen.x), screen.y - 800});
        m_next_chaser_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
    }

    for (auto &m_chaser : m_chasers)
        m_chaser->update(this, elapsed_ms);

    //////////////SPAWNDONE/////////////////
    //ASTAR
    float width = m_size.x / COL;
    float height = m_size.y / ROW;
    int grid[ROW][COL];

    if (!isGraphCreated) {
        for (int p = 0; p < ROW; p++) {
            for (int h = 0; h < COL; h++) {
                grid[p][h] = 1;
            }
        }
        isGraphCreated = true;
    }

/*    for (auto &m_bomb : m_bomberBombs){
        int j = 0;
        int l = 0;
        for (float k = 50.f*//*0.f*//*; k <= 5000.f *//*1200.f*//*; k += width) {
            for (float i = *//*0*//*50.f; i <= 1000.f; i += height) {
                if (m_bomb.getPosition().y >= 50.f && m_bomb.getPosition().y < height
                    && m_bomb.getPosition().x >= 50.f && m_bomb.getPosition().x < width) {
                    //  Pair src = make_pair(0, 0);
                    j = 0;
                    l = 0;
                    grid[j][l] = 0;
                    grid[j+2][l] = 0;
                    grid[j-2][l] = 0;
                    grid[j][l+2] = 0;
                    grid[j][l+2] = 0;
                    grid[j+2][l+2] = 0;
                    grid[j-2][l+2] = 0;
                    grid[j+2][l-2] = 0;
                    grid[j+2][l-2] = 0;

                   // printf("FOUND!");


                } else if ((m_bomb->getPosition().y >= (i) && m_bomb->getPosition().y < (i + height))
                           && (m_bomb->getPosition().x >= (k) && m_bomb->getPosition().x < (k + width))) {
                    // Pair src = make_pair(j,l);
                    grid[j][l] = 0;
                    grid[j+2][l] = 0;
                    grid[j-2][l] = 0;
                    grid[j][l+2] = 0;
                    grid[j][l+2] = 0;
                    grid[j+2][l+2] = 0;
                    grid[j-2][l+2] = 0;
                    grid[j+2][l-2] = 0;
                    grid[j+2][l-2] = 0;

                   // printf("FOUND!");
                }
                j++;
            }

            l++;
            j = 0;
        }
    }*/

    for (auto &m_chaser : m_chasers) {
        bool srcFound = false;
        bool destFound = false;

        int j = 0;
        int l = 0;
        if (!srcFound) {
            for (float k = 50.f/*0.f*/; k <= 3000.f /*1200.f*/; k += width) {
                for (float i = /*0*/50.f; i <= 1500.f; i += height) {
                    if (m_chaser->getPosition().y >= 50.f && m_chaser->getPosition().y < height
                        && m_chaser->getPosition().x >= 50.f && m_chaser->getPosition().x < width) {
                        //  Pair src = make_pair(0, 0);
                        j = 0;
                        l = 0;
                        srcFound = true;
                        if (srcFound) {
                            break;
                        }

                    } else if ((m_chaser->getPosition().y >= (i) && m_chaser->getPosition().y < (i + height))
                               && (m_chaser->getPosition().x >= (k) && m_chaser->getPosition().x < (k + width))) {
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
            for (float k = /*0*/50.f; k <= /*1200*/3000.f; k += width) {
                for (float i = 50.f; i <= 1500.f; i += height) {
                    if (m_player.getPosition().y >= 50.f && m_player.getPosition().y < height
                        && m_player.getPosition().x >= 50.f && m_player.getPosition().x < width) {
                        //Pair dest = make_pair(0, 0);
                        a = 0;
                        b = 0;
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

        if (destFound && srcFound) {
            Pair src = std::make_pair(j, l);
            Pair dest = std::make_pair(a, b);
            m_chaser->aStarSearch(grid, src, dest);
        }

    }

    ////////CHASER DONE//////////

    ////////BOMBER LOGIC///////

    // Spawing the bomber
    m_next_bomber_spawn -= elapsed_ms;
    if (m_bombers.size() < MAX_BOMBER && m_next_bomber_spawn) {
        if (auto newBomber = Bomber::spawn()) {
            m_bombers.emplace_back(newBomber);
        }
        auto newBomberPtr = m_bombers.back();
        // Setting initial position on top
        newBomberPtr->setPosition({50 + m_dist(m_rng) * (screen.x), screen.y - 800});
        // Next spawn
        m_next_bomber_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
    }

    // move bomber in horizontal direction
    for (auto &m_bomber : m_bombers)
        m_bomber->update(this, elapsed_ms);

    // spawn Bomber bombs
    for (auto &m_bomber : m_bombers){
        if(bomberOnScreen(*m_bomber)){
            m_next_bbomb_spawn -= elapsed_ms;
            if (m_bomberBombs.size() < MAX_BOMBERBOMBS && m_next_bbomb_spawn < 0.f) {
                if (auto newBomb = BomberBomb::spawn()) {
                    m_bomberBombs.emplace_back(newBomb);
                }

                auto newBombPtr = m_bomberBombs.back();
                newBombPtr->setPosition(getPlayerPosition());

                m_next_bbomb_spawn = (BOMB_DELAY_MS * 2) + m_dist(m_rng) * (BOMB_DELAY_MS * 2);
            }
        }
    }

    // trigger bomber bomb animation
    for(auto &bomb : m_bomberBombs)
        bomb->update(elapsed_ms);

    // remove bomber bombs from screen
    auto bomberBomb_it = m_bomberBombs.begin();
    while (bomberBomb_it != m_bomberBombs.end()) {
        int fc = (*bomberBomb_it)->getFrameCount();
        if (fc == 0) {
            bomberBomb_it = m_bomberBombs.erase(bomberBomb_it);
            continue;
        }

        ++bomberBomb_it;
    }

    //////////BOMBER DONE/////////

    // trigger normal bomb animation
    for (auto &bomb : m_normalBombs)
        bomb->update(elapsed_ms);

    // removing normal bombs from screen
    auto normalBomb_it = m_normalBombs.begin();
    while (normalBomb_it != m_normalBombs.end()) {
        int fc = (*normalBomb_it)->getFrameCount();
        if (fc == 0) {
            normalBomb_it = m_normalBombs.erase(normalBomb_it);
            continue;
        }

        ++normalBomb_it;
    }

    // Spawn new normal bombs
    m_next_nbomb_spawn -= elapsed_ms;
    if (m_normalBombs.size() < MAX_BOMBS && m_next_nbomb_spawn < 0.f) {

        if (auto newNormalBomb = NormalBomb::spawn()) {
            m_normalBombs.emplace_back(newNormalBomb);
        }

        auto newNormalBombPtr = m_normalBombs.back();

        newNormalBombPtr->setPosition({50 + m_dist(m_rng) * (screen.x), m_dist(m_rng) * (screen.y)});
        m_next_nbomb_spawn = (BOMB_DELAY_MS) + m_dist(m_rng) * (BOMB_DELAY_MS);
    }

    //////COLLISION DETECTION/////

    // collision detection between shooter and player bullet
    playerBulletIt = m_player.getBullets().begin();
    while (playerBulletIt != m_player.getBullets().end()) {
        bool isColliding = false;
        auto benemy_it = m_shooters.begin();
        while (benemy_it != m_shooters.end()) {
            if ((*playerBulletIt)->collisionCheck(**benemy_it)) {
                benemy_it = m_shooters.erase(benemy_it);
                playerBulletIt = m_player.getBullets().erase(playerBulletIt);
                isColliding = true;
                m_points = m_points + 5;
                break;
            }
            ++benemy_it;
        }
        if (!isColliding) {
            ++playerBulletIt;
        }
    }

    // collision detection between normal bomb and player bullet
    playerBulletIt = m_player.getBullets().begin();
    while (playerBulletIt != m_player.getBullets().end()) {
        bool isColliding = false;
        for (auto &bomb : m_normalBombs) {
            if ((*playerBulletIt)->collisionCheck(*bomb)) {
                playerBounce(*bomb);
                bomb->animate();
                playerBulletIt = m_player.getBullets().erase(playerBulletIt);
                isColliding = true;
                m_points = m_points + 5;
                break;
            }
        }
        if (!isColliding) {
            ++playerBulletIt;
        }
    }

    // collision detection between normal bomb and shooter bullet
    for (auto &shooter : m_shooters) {
        auto shooterBulletIt = shooter->getBullets().begin();
        while (shooterBulletIt != shooter->getBullets().end()) {
            bool isColliding = false;
            for (auto &bomb : m_normalBombs) {
                if ((*shooterBulletIt)->collisionCheck(*bomb)){
                    playerBounce(*bomb);
                    bomb->animate();
                    shooterBulletIt = shooter->getBullets().erase(shooterBulletIt);
                    isColliding = true;
                    break;
                }
            }
            if (!isColliding) {
                ++shooterBulletIt;
            }
        }
    }

    // collision detection between player bullet and chaser
    playerBulletIt = m_player.getBullets().begin();
    while (playerBulletIt != m_player.getBullets().end()) {
        bool chaserCol = false;
        auto chaserIt = m_chasers.begin();
        while (chaserIt != m_chasers.end()) {
            if ((*playerBulletIt)->collisionCheck(**chaserIt)) {
                chaserIt = m_chasers.erase(chaserIt);
                playerBulletIt = m_player.getBullets().erase(playerBulletIt);
                chaserCol = true;
                m_points = m_points + 10;
                break;
            }
            ++chaserIt;
        }
        if (!chaserCol) {
            ++playerBulletIt;
        }
    }

    // collision detection between player bullet and bomber
    playerBulletIt = m_player.getBullets().begin();
    while (playerBulletIt != m_player.getBullets().end()) {
        bool bomberColliding = false;
        auto bomber_it = m_bombers.begin();
        while (bomber_it != m_bombers.end()) {
            if ((*playerBulletIt)->collisionCheck(**bomber_it)) {
                bomber_it = m_bombers.erase(bomber_it);
                playerBulletIt = m_player.getBullets().erase(playerBulletIt);
                bomberColliding = true;
                m_points = m_points + 10;
                break;
            }
            ++bomber_it;
        }
        if (!bomberColliding) {
            ++playerBulletIt;
        }
    }

    // collision detection between player and bomber bomb
    bomberBomb_it = m_bomberBombs.begin();
    while (bomberBomb_it != m_bomberBombs.end()) {
        bool isColliding = false;
        if(((*bomberBomb_it)->isBlasting()) && m_player.collisionCheck(**bomberBomb_it)){
            isColliding = true;
            m_player.hit();
            break;
        }
        if(!isColliding){
            ++bomberBomb_it;
        }
    }


    ////COLLISION DONE////


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
        m_chaser->draw(projection_2D);

    for (auto &shooter : m_shooters) {
        shooter->draw(projection_2D);
        for (auto &bullet : shooter->getBullets()) {
            bullet->draw(projection_2D);
        }
    }

    for (auto &bullet : m_player.getBullets()) {
        bullet->draw(projection_2D);
    }


    for (auto &nBomb : m_normalBombs) {
        nBomb->draw(projection_2D);
    }

    for (auto &bBomb : m_bomberBombs) {
        bBomb->draw(projection_2D);
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
    for (auto &bomb : m_normalBombs) {
        positions.emplace_back(bomb->getPosition());
    }
    return positions;
}

vec2 World::getCityPosition() const {
    return m_background.getPosition();
}

// Private

bool World::initTextures() {
    return BomberBomb::initTexture() &&
           NormalBomb::initTexture() &&
           Shooter::initTexture() &&
           Chaser::initTexture() &&
           Bomber::initTexture() &&
           PlayerBullet::initTexture() &&
           ShooterBullet::initTexture() &&
           background::initTexture();
}

void World::playerBounce(NormalBomb bomb) {
    float bounceBackSpeed = -80.f;
    bulletAngleRelativeToPlayer = m_player.getRotation() + 3.1415f / 2.f;
    bulletDirectionRelativeToPlayer = {cosf(bulletAngleRelativeToPlayer), sinf(bulletAngleRelativeToPlayer)};

    float diffX = m_player.getPosition().x - bomb.getPosition().x;
    float diffY = m_player.getPosition().x - bomb.getPosition().x;
    vec2 diff = {diffX, diffY};
    float distance = magnitude(diff);
    if (distance < 200.f) {
        vec2 bounceBackDist = {(bounceBackSpeed * bulletDirectionRelativeToPlayer.x),
                               (bounceBackSpeed * bulletDirectionRelativeToPlayer.y)};
        m_player.move(bounceBackDist);
    }
}

bool World::bomberOnScreen(Bomber &bomber) {
    return ((bomber.getPosition().y < m_camera.getBottomBoundary()) ||
        (bomber.getPosition().y > m_camera.getTopBoundary()) ||
        (bomber.getPosition().x < m_camera.getRightBoundary()) ||
        (bomber.getPosition().x > m_camera.getLeftBoundary()));
}

// On key callback
void World::onKey(GLFWwindow *, int key, int, int action, int mod) {
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            m_player.setFlying(Player::DIRECTION::FORWARD, true);
        } else if (action == GLFW_RELEASE) {
            m_player.setFlying(Player::DIRECTION::FORWARD, false);
        }
    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            m_player.setFlying(Player::DIRECTION::BACKWARD, true);
        } else if (action == GLFW_RELEASE) {
            m_player.setFlying(Player::DIRECTION::BACKWARD, false);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            m_player.setFlying(Player::DIRECTION::LEFT, true);
        } else if (action == GLFW_RELEASE) {
            m_player.setFlying(Player::DIRECTION::LEFT, false);
        }
    }

    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            m_player.setFlying(Player::DIRECTION::RIGHT, true);
        } else if (action == GLFW_RELEASE) {
            m_player.setFlying(Player::DIRECTION::RIGHT, false);
        }
    }

    // Resetting game
    if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
        int w, h;
        glfwGetWindowSize(m_window, &w, &h);
        m_player.init(m_size);
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