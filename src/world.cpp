// Header
#include "world.hpp"
#include "states/information.hpp"
#include "states/gameOver.hpp"



// stlib
#include <sstream>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <GL/gl.h>

typedef pair<int, int> Pair;

// Same as static in c, local to compilation unit
namespace {

    size_t MAX_BOMBS = 0;
    size_t MAX_BOMBERBOMBS = 0;
    size_t MAX_SHOOTERS = 1;
    size_t MAX_CHASER = 0;
    size_t MAX_BOMBER = 0;
    size_t MAX_POWERUP = 1;
    int bombs = MAX_BOMBS;
    int bBombs = MAX_BOMBERBOMBS;
    int shooters = MAX_SHOOTERS;
    int chasers = MAX_CHASER;
    int bombers = MAX_BOMBER;
    const size_t SHOOTER_DELAY_MS = 2000;
    const size_t BOMB_DELAY_MS = 2000;
    const size_t CHASER_DELAY_MS = 10000;
    const size_t POWERUP_DELAY_MS = 2000;


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
        m_next_bbomb_spawn(0.f),
        m_next_oneup_spawn(0.f),
        m_next_shield_spawn(0.f),
        m_camera({}, {}),
        m_quad(0, {}) {
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
    m_window = glfwCreateWindow((int) screenSize.x, (int) screenSize.y, "DefendVancouver", nullptr,
                                nullptr);
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


    state = 0;

    waveNo = 1;
    m_size = worldSize;
    m_camera = Camera(screenSize, worldSize);
    m_quad = QuadTreeNode(0, {{0.f, 0.f}, worldSize});
    initTextures();
    totalEnemies = shooters + chasers;
    auto bg = std::make_shared<background>(*this);
    bg->init();
    addEntity(bg);
    auto player = std::make_shared<Player>(*this);
    player->init();
    addEntity(player);
    auto start = std::make_shared<StartScreen>(*this);
    start->setPosition(
            {m_camera.getLeftBoundary() + (screenSize.x / 2), m_camera.getTopBoundary() + (screenSize.y / 2)});
    start->init();
    addState(start);
    addState(bg);
    auto info = std::make_shared<Info>(*this);
    info->setPosition(
            {m_camera.getLeftBoundary() + (screenSize.x / 2), m_camera.getTopBoundary() + (screenSize.y / 2)});
    info->init();
    addState(info);
    auto over = std::make_shared<GameOver>(*this);
    over->setPosition(
            {m_camera.getLeftBoundary() + (screenSize.x / 2), m_camera.getTopBoundary() + (screenSize.y / 2)});
    over->init();
    addState(over);

    width = m_size.x / COL;
    height = m_size.y / ROW;
    grid[ROW][COL];

    if (!isGraphCreated) {
        for (int p = 0; p < ROW; p++) {
            for (int h = 0; h < COL; h++) {
                grid[p][h] = 1;
            }
        }
        isGraphCreated = true;
    }

    return true;
}

// Releases all the associated resources
void World::destroy() {
    glfwDestroyWindow(m_window);
}

// Update our game world
void World::update(float elapsed_ms) {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    vec2 screen = {(float) w, (float) h};

    // Setting wave spawn conditions
    if (totalEnemies <= 0) {
        waveNo++;
        if (waveNo % 3 == 1) {
            MAX_SHOOTERS++;
        }

        if (waveNo % 2 == 1) {
            MAX_CHASER++;
            MAX_BOMBER++;
            MAX_BOMBERBOMBS++;
            MAX_BOMBS++;
        }
        shooters = MAX_SHOOTERS;
        chasers = MAX_CHASER;
        bombers = MAX_BOMBER;
        bBombs = MAX_BOMBERBOMBS;
        bombs = MAX_BOMBS;
        totalEnemies = shooters + chasers;
    }

    for (auto &state: m_states){
        if (state->getName() == "background"){
            continue;
        } else {
            state->setPosition(
                    {m_camera.getLeftBoundary() + (screen.x / 2), m_camera.getTopBoundary() + (screen.y / 2)});
        }
    }

    //////COLLISION DETECTION/////
    m_quad.clear();

    for (auto &entity: m_entities) {
        entity->update(elapsed_ms);
        m_quad.insert(entity);
    }

    m_camera.update(elapsed_ms, getPlayerPosition());

    // once everything is inserted, go through each entity and get vector of nearby entities
    // that could possibly collide with that entity
    for (auto &entity: m_entities) {
        auto nearbyEntities = m_quad.getNearbyEntities(entity);
        for (auto &nearbyEntity: nearbyEntities) {
            // run collision detection between entities
            if (typeid(*entity) == typeid(PlayerBullet)) {
                if (entity->isCollidingWith(*nearbyEntity)) {
                    // Handle collision based on nearbyEntity's type
                    if (typeid(*nearbyEntity) == typeid(Shooter)) {
                        totalEnemies--;
                        m_points += 5;
                        nearbyEntity->die();
                        entity->die();
                    } else if (typeid(*nearbyEntity) == typeid(NormalBomb)) {
                        totalEnemies--;
                        playerBounce(*(std::dynamic_pointer_cast<NormalBomb>(nearbyEntity)));
                        std::dynamic_pointer_cast<NormalBomb>(nearbyEntity)->animate();
                        entity->die();
                    } else if (typeid(*nearbyEntity) == typeid(Chaser)) {
                        totalEnemies--;
                        m_points += 10;
                        nearbyEntity->die();
                        entity->die();
                    } else if (typeid(*nearbyEntity) == typeid(Bomber)) {
                        m_points += 10;
                        nearbyEntity->die();
                        entity->die();
                    }
                }
            } else if (typeid(*entity) == typeid(ShooterBullet)) {
                if (entity->isCollidingWith(*nearbyEntity)) {
                    if (typeid(*nearbyEntity) == typeid(Player)) {
                        entity->die();
                        getPlayer()->hit();
                    }
//                    if (typeid(*nearbyEntity) == typeid(background)) {
//                        entity->die();
//                    }
                }
            } else if (typeid(*entity) == typeid(Player)) {
                if (entity->isCollidingWith(*nearbyEntity)) {
                    if (typeid(*nearbyEntity) == typeid(BomberBomb) &&
                        std::dynamic_pointer_cast<BomberBomb>(nearbyEntity)->isBlasting()) {
                        getPlayer()->hit();
                    } else if (typeid(*nearbyEntity) == typeid(OneUp)) {
                        getPlayer()->addLives();
                        nearbyEntity->die();
                    } else if (typeid(*nearbyEntity) == typeid(Shield)) {
                        getBackground()->addHealth();
                        nearbyEntity->die();
                    } else if (typeid(*nearbyEntity) == typeid(Shooter)) {
                        getPlayer()->hit();
                    } else if (typeid(*nearbyEntity) == typeid(Chaser)) {
                        getPlayer()->hit();
                    } else if (typeid(*nearbyEntity) == typeid(Bomber)) {
                        getPlayer()->hit();
                    }
                }
            }
        }
    }


//ASTAR
    for (auto &entity : m_entities) {
        if (typeid(*entity) == typeid(Chaser)) {
            entity->update(elapsed_ms);
        }
    }

    //// CLEANUP ////

    auto entityIt = m_entities.begin();
    while (entityIt != m_entities.end()) {
        if ((*entityIt)->isDead()) {
            entityIt = m_entities.erase(entityIt);
            continue;
        }
        ++entityIt;
    }

    //// SPAWNING ////

    m_next_shooter_spawn -= elapsed_ms;
    if (m_next_shooter_spawn < 0.f && shooters != 0) {
        auto newShooter = Shooter::spawn(*this);
        // Setting random initial position
        newShooter->setPosition({m_dist(m_rng) * m_size.x, -200.f});
        // Next spawn
        m_next_shooter_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
        shooters--;
    }

    // bullet spawning

    // create copy vector which is not affected during iteration
    auto entities = m_entities;
    for (auto &entity : entities) {
        if (typeid(*entity) == typeid(Player)) {
            auto player = std::dynamic_pointer_cast<Player>(entity);
            if (player->isShooting()) {
                player->shoot();
            }
        } else if (typeid(*entity) == typeid(Shooter)) {
            std::dynamic_pointer_cast<Shooter>(entity)->shoot();
        }
    }


    m_next_chaser_spawn -= elapsed_ms;
    if (m_next_chaser_spawn < 0.f && chasers != 0) {
        auto newChaser = Chaser::spawn(*this);
        // Setting random initial position
        newChaser->setPosition({50 + m_dist(m_rng) * (screen.x), screen.y - 800});
        m_next_chaser_spawn = (CHASER_DELAY_MS / 2) + m_dist(m_rng) * (CHASER_DELAY_MS / 2);
        chasers--;
    }

    m_next_bomber_spawn -= elapsed_ms;
    if (m_next_bomber_spawn < 0.f && bombers != 0) {
        auto newBomber = Bomber::spawn(*this);
        // Setting initial position on top
        newBomber->setPosition({50 + m_dist(m_rng) * (screen.x), screen.y - 800});
        // Next spawn
        m_next_bomber_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
        bombers--;
    }

    // create copy vector which is not affected during iteration
    entities = m_entities;
    for (auto &entity : entities) {
        if (typeid(*entity) == typeid(Bomber)) {
            if (m_camera.isEntityInView(*entity)) {
                m_next_bbomb_spawn -= elapsed_ms;
                if (m_next_bbomb_spawn < 0.f) {
                    auto newBomb = BomberBomb::spawn(*this);
                    newBomb->setPosition(getPlayerPosition());
                    m_next_bbomb_spawn = (BOMB_DELAY_MS * 2) + m_dist(m_rng) * (BOMB_DELAY_MS * 2);
                    bBombs--;
                }
            }
        }
    }

    // Spawn new normal bombs
    m_next_nbomb_spawn -= elapsed_ms;
    if (m_next_nbomb_spawn < 0.f && bombs != 0) {
        auto newNormalBomb = NormalBomb::spawn(*this);
        newNormalBomb->setPosition({50 + m_dist(m_rng) * (screen.x), m_dist(m_rng) * (screen.y)});
        m_next_nbomb_spawn = (BOMB_DELAY_MS) + m_dist(m_rng) * (BOMB_DELAY_MS);
        bombs--;
    }

    // spawn oneups
    m_next_oneup_spawn -= elapsed_ms;
    if (MAX_POWERUP != 0 && m_next_oneup_spawn < 0.f) {
        auto newOneUp = OneUp::spawn(*this);
        newOneUp->setPosition({m_dist(m_rng) * m_size.x, -200.f});
        m_next_oneup_spawn = (POWERUP_DELAY_MS) + m_dist(m_rng) * (POWERUP_DELAY_MS);
    }

    // spawn shield
    m_next_shield_spawn -= elapsed_ms;
    if (MAX_POWERUP != 0 && m_next_shield_spawn < 0.f) {
        auto newShield = Shield::spawn(*this);
        newShield->setPosition({m_dist(m_rng) * m_size.x, -200.f});
        m_next_shield_spawn = (POWERUP_DELAY_MS) + m_dist(m_rng) * (POWERUP_DELAY_MS);
    }
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
    if (state == 1) {
        // Updating window title with points
        std::stringstream title_ss;
        title_ss << "Points: " << m_points << " Lives: " << getPlayer()->getLives() << " City: "
                 << getBackground()->getHealth()
                 << " s: " << shooters << " c: " << chasers
                 << " b: " << bombers << " Wave: " << waveNo << " t: " << totalEnemies;
        glfwSetWindowTitle(m_window, title_ss.str().c_str());

        for (auto &entity: m_entities) entity->draw(projection_2D);
    } else {
        m_states[state]->draw(projection_2D);
    }

    // Presenting
    glfwSwapBuffers(m_window);
}

// Should the game be over ?
bool World::is_over() const {
    return glfwWindowShouldClose(m_window);
}

vec2 World::getPlayerPosition() const {
    return (*m_entities.at(1)).getPosition();
}

std::vector<vec2> World::getBombPositions() const {
    auto positions = std::vector<vec2>();
    for (auto &entity : m_entities) {
        if (typeid(*entity) == typeid(NormalBomb)) {
            positions.emplace_back(entity->getPosition());
        }
    }
    return positions;
}

vec2 World::getCityPosition() const {
    return (*m_entities.front()).getPosition();
}

void World::addEntity(std::shared_ptr<Entity> entity) {
    m_entities.emplace_back(entity);
}

vec2 World::getSize() const {
    return m_size;
}

bool World::isEntityInView(const Entity &entity) const {
    return m_camera.isEntityInView(entity);
}

// Private

bool World::initTextures() {
    return BomberBomb::initTexture() &&
           NormalBomb::initTexture() &&
           OneUp::initTexture() &&
           Shield::initTexture() &&
           Shooter::initTexture() &&
           Chaser::initTexture() &&
           Bomber::initTexture() &&
           PlayerBullet::initTexture() &&
           ShooterBullet::initTexture() &&
           background::initTexture() &&
           StartScreen::initTexture() &&
           Info::initTexture() &&
           GameOver::initTexture();
}

std::shared_ptr<Player> World::getPlayer() const {
    return std::dynamic_pointer_cast<Player>(m_entities.at(1));
}

std::shared_ptr<background> World::getBackground() const {
    return std::dynamic_pointer_cast<background>(m_entities.front());
}

void World::playerBounce(const NormalBomb &bomb) {
    float bounceBackSpeed = -80.f;
    bulletAngleRelativeToPlayer = getPlayer()->getRotation() + 3.1415f / 2.f;
    bulletDirectionRelativeToPlayer = {cosf(bulletAngleRelativeToPlayer), sinf(bulletAngleRelativeToPlayer)};

    float diffX = getPlayer()->getPosition().x - bomb.getPosition().x;
    float diffY = getPlayer()->getPosition().x - bomb.getPosition().x;
    vec2 diff = {diffX, diffY};
    float distance = magnitude(diff);
    if (distance < 200.f) {
        vec2 bounceBackDist = {(bounceBackSpeed * bulletDirectionRelativeToPlayer.x),
                               (bounceBackSpeed * bulletDirectionRelativeToPlayer.y)};
        getPlayer()->move(bounceBackDist);
    }
}

// On key callback
void World::onKey(GLFWwindow *, int key, int, int action, int mod) {
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            getPlayer()->setFlying(Player::DIRECTION::FORWARD, true);
        } else if (action == GLFW_RELEASE) {
            getPlayer()->setFlying(Player::DIRECTION::FORWARD, false);
        }
    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            getPlayer()->setFlying(Player::DIRECTION::BACKWARD, true);
        } else if (action == GLFW_RELEASE) {
            getPlayer()->setFlying(Player::DIRECTION::BACKWARD, false);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            getPlayer()->setFlying(Player::DIRECTION::LEFT, true);
        } else if (action == GLFW_RELEASE) {
            getPlayer()->setFlying(Player::DIRECTION::LEFT, false);
        }
    }

    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            getPlayer()->setFlying(Player::DIRECTION::RIGHT, true);
        } else if (action == GLFW_RELEASE) {
            getPlayer()->setFlying(Player::DIRECTION::RIGHT, false);
        }
    }

    if (key == GLFW_KEY_P || (key == GLFW_KEY_P && state == 3)) {
        if (action == GLFW_PRESS) {
            std::cout << "pressed P" << std::endl;
            state = 1;
            std::cout << state << std::endl;
        }
    }

    if (key == GLFW_KEY_ESCAPE) {
        if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, 1);
            is_over();
        }
    }

    if (key == GLFW_KEY_I) {
        if (action == GLFW_PRESS) {
            state = 2;
        }
    }

    if (key == GLFW_KEY_B && state == 2) {
        if (action == GLFW_PRESS) {
            state = 0;
        }
    }

    // Resetting game
    if (getPlayer().get()->m_lives < 1) {
        state = 3;

    }

}


void World::onMouseMove(GLFWwindow *window, double xpos, double ypos) {
    vec2 playerScreenPos = {
            getPlayer()->getPosition().x - m_camera.getLeftBoundary(),
            getPlayer()->getPosition().y - m_camera.getTopBoundary()
    };
    auto playerMouseXDist = float(xpos - playerScreenPos.x);
    auto playerMouseYDist = float(ypos - playerScreenPos.y);
    float newOrientation = -1.f * atan((playerMouseXDist / playerMouseYDist));
    if (playerMouseYDist < 0.f) newOrientation += 3.1415f;
    getPlayer()->setRotation(newOrientation);
}

void World::onMouseClick(GLFWwindow *window, int button, int action, int mod) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            getPlayer()->enableShooting(true);
        } else if (action == GLFW_RELEASE) {
            getPlayer()->enableShooting(false);
        }
    }
}

int World::getState() {
    return state;
}

void World::addState(std::shared_ptr<Entity> entity) {
    m_states.emplace_back(entity);
}



