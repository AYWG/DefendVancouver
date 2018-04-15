// Header
#include "world.hpp"
#include "../stb-cmake/stb_image.h"



// stlib
#include <sstream>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <GL/gl3w.h>
#include <typeinfo>


typedef pair<int, int> Pair;
// Same as static in c, local to compilation unit
namespace {

    size_t MAX_BOMBERBOMBS = 0;
    size_t MAX_BOMBS = 0;
    size_t MAX_SHOOTERS = 20;
    size_t MAX_CHASER = 0;
    size_t MAX_BOMBER = 0;
    int remainingNormalBombsToSpawn = MAX_BOMBS;
    int remainingShootersToSpawn = MAX_SHOOTERS;
    int remainingChasersToSpawn = MAX_CHASER;
    int remainingBombersToSpawn = MAX_BOMBER;
    const size_t SHOOTER_DELAY_MS = 2000;
    const size_t BOMB_DELAY_MS = 4000;
    const size_t CHASER_DELAY_MS = 5000;
    size_t POWERUP_DELAY_MS = 10000;
    const size_t MIN_POWERUP_DELAY_MS = 1000;

    namespace {
        void glfw_err_cb(int error, const char *desc) {
//            fprintf(stderr, "%d: %s", error, desc);
        }
    }



}

World::World() :
        m_points(0),
        m_next_shooter_spawn(0.f),
        m_next_chaser_spawn(0.f),
        m_next_bomber_spawn(0.f),
        m_next_normal_bomb_spawn(0.f),
        m_next_powerup_spawn(0.f),
        m_camera({}, {}),
        m_ui({}, *this),
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
//        fprintf(stderr, "Failed to initialize GLFW");
        return false;
    }

    typeEnemy = typeid(Enemy).name();



    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
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

    int width, height;
    stbi_uc *data = stbi_load(textures_path("crosshair.png"), &width, &height, NULL, 4);
    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = data;

    GLFWcursor *cursor = glfwCreateCursor(&image, 0, 0);
    glfwSetCursor(m_window, cursor);

    m_state = 0;
    m_prevState = m_state - 1;

    if (!initScore()) return false;

    m_invincibility = false;
    m_waveNo = 1;
    m_size = worldSize;
    m_camera = Camera(screenSize, worldSize);
    m_ui = UI(screenSize, *this);
    m_ui.init();
    m_quad = QuadTreeNode(0, {{0.f, 0.f}, worldSize});
    initGraphics();
    m_remainingEnemiesInWave = remainingShootersToSpawn + remainingChasersToSpawn + remainingBombersToSpawn;
    m_stars.init();
    auto bg = std::make_shared<background>(*this);
    bg->init();
    addEntity(bg);
    auto player = std::make_shared<Player>(*this);
    player->init();
    addEntity(player);
    auto start = std::make_shared<StartScreen>(*this);
    start->setPosition(m_camera.getFocusPoint());
    start->init();
    auto info = std::make_shared<Info>(*this);
    info->setPosition(m_camera.getFocusPoint());
    info->init();
    auto over = std::make_shared<GameOver>(*this);
    over->init();
    auto high = std::make_shared<HighScore>(*this);
    high->init();
    addState(start);
    addState(bg);
    addState(info);
    addState(over);
    addState(high);




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



    // Enable the debug callback
  /*  glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
    );*/


    return true;
}

// Releases all the associated resources
void World::destroy() {

    for (auto &entity : m_entities) {
        entity->destroy();
    }
    fclose(m_scoreFile);
}

// Update our game world
void World::update(float elapsed_ms) {

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    vec2 screen = {(float) w, (float) h};

    if (m_state == 3){
        gameOver = true;
    } else
        gameOver = false;

    if (gameOver){
        m_ui.init();
        gameOver == false;
    }
    // Resetting game
    if (getPlayer().get()->gameOver) {
        if (m_points > m_bestScore) {
            m_bestScore = m_points;
            m_scoreFile = fopen(scores_path("score.txt"), "w+");
            fprintf(m_scoreFile, "%d", m_bestScore);
            fclose(m_scoreFile);
        }
        m_state = 3;
        m_ui.destroy();

    }
    if (m_remainingEnemiesInWave <= 0) {
        advanceWave();
    }

    for (auto &state: m_states) {
        if (state->getName() == "background") {
            continue;
        } else {
            state->setPosition(m_camera.getFocusPoint());
        }
    }

    auto particle = std::make_shared<shipParticle>(*this);
    //////COLLISION DETECTION/////
    m_quad.clear();

    for (auto &entity: m_entities) {
        entity->update(elapsed_ms);
        m_quad.insert(entity);
    }

    m_camera.update(elapsed_ms, getPlayerPosition());
    m_ui.update(elapsed_ms);

    // once everything is inserted, go through each entity and get vector of nearby entities
    // that could possibly collide with that entity
    for (auto &entity: m_entities) {
        auto nearbyEntities = m_quad.getNearbyEntities(entity);
        for (auto &nearbyEntity: nearbyEntities) {
            // run collision detection between entities

            if (entity->isCollidingWith(*nearbyEntity)) {
                entity->onCollision(*nearbyEntity);
                nearbyEntity->onCollision(*entity);

            }
        }
    }
    //is_shot = false;
    //// CLEANUP ////

    auto entityIt = m_entities.begin();
    while (entityIt != m_entities.end()) {

        if ((*entityIt)->isDead()) {

            if ((entityIt->get()->getName() == "Shooter") ||
                (entityIt->get()->getName() == "Chaser") ||
                (entityIt->get()->getName() == "Bomber") ||
                (entityIt->get()->getName() == "BomberBomb") ||
                (entityIt->get()->getName() == "NormalBomb")) {
                particle->setPosition(entityIt->get()->getPosition());
                is_shot = true;
            }

            entityIt = m_entities.erase(entityIt);

            continue;
        }

        ++entityIt;
    }

    if (isShot()) {
        particle->init();
        addEntity(particle);
        is_shot = false;

    }


    //// SPAWNING ////

    m_next_shooter_spawn -= elapsed_ms;
    if (m_next_shooter_spawn < 0.f && remainingShootersToSpawn > 0) {
        auto newShooter = Shooter::spawn(*this);
        // Setting random initial position
        newShooter->setPosition({m_dist(m_rng) * m_size.x, -200.f});
        // Next spawn
        m_next_shooter_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
        remainingShootersToSpawn--;
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
    if (m_next_chaser_spawn < 0.f && remainingChasersToSpawn > 0) {
        auto newChaser = Chaser::spawn(*this);
        // Setting random initial position
        newChaser->setPosition({50 + m_dist(m_rng) * (screen.x), screen.y - 800});
        m_next_chaser_spawn = (CHASER_DELAY_MS / 2) + m_dist(m_rng) * (CHASER_DELAY_MS / 2);
        remainingChasersToSpawn--;
    }

    m_next_bomber_spawn -= elapsed_ms;
    if (m_next_bomber_spawn < 0.f && remainingBombersToSpawn > 0) {
        auto newBomber = Bomber::spawn(*this);
        newBomber->setPosition({ -100, m_dist(m_rng) * 1000});
        // Next spawn
        m_next_bomber_spawn = (SHOOTER_DELAY_MS / 2) + m_dist(m_rng) * (SHOOTER_DELAY_MS / 2);
        remainingBombersToSpawn--;
    }

    // create copy vector which is not affected during iteration
    entities = m_entities;
    for (auto &entity : entities) {
        if (typeid(*entity) == typeid(Bomber)) {
            std::dynamic_pointer_cast<Bomber>(entity)->plantBomb();
        }
    }

    // Spawn new normal bombs
    m_next_normal_bomb_spawn -= elapsed_ms;
    if (m_next_normal_bomb_spawn < 0.f && remainingNormalBombsToSpawn != 0) {
        auto newNormalBomb = NormalBomb::spawn(*this);
        newNormalBomb->setPosition({50 + m_dist(m_rng) * (screen.x), m_dist(m_rng) * (screen.y)});
        m_next_normal_bomb_spawn = (BOMB_DELAY_MS) + m_dist(m_rng) * (BOMB_DELAY_MS);
        remainingNormalBombsToSpawn--;
    }

    // Spawn powerups (random)
    m_next_powerup_spawn -= elapsed_ms;
    if (m_next_powerup_spawn < 0.f) {
        auto rng = m_dist(m_rng);
        std::shared_ptr<PowerUp> powerUp;
        if (rng >= 0.66) {
            powerUp = OneUp::spawn(*this);
        } else if (rng >= 0.33) {
            powerUp = CityUp::spawn(*this);
        }
        else {
            powerUp = Shield::spawn(*this);
        }
        powerUp->setPosition({m_dist(m_rng) * m_size.x, -200.f});
        m_next_powerup_spawn = (POWERUP_DELAY_MS) + m_dist(m_rng) * (POWERUP_DELAY_MS);
    }

    // Invincibility
    if (m_invincibility) {
        m_invincibility_countdown -= elapsed_ms;
    }

    if (m_invincibility_countdown <= 0.f) {
        m_invincibility = false;
        getPlayer()->setInvincibility(m_invincibility);
        getBackground()->setInvincibility(m_invincibility);
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

    // Fake projection matrix
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

    m_stars.draw(projection_2D);

    // Fake projection matrix for UI with respect to window coordinates
    float lUI = 0.f;// *-0.5;
    float tUI = 0.f;// (float)h * -0.5;
    float rUI = (float) w;// *0.5;
    float bUI = (float) h;// *0.5;
    float sX = 2.f / (rUI - lUI);
    float sY = 2.f / (tUI - bUI);
    float tX = -(rUI + lUI) / (rUI - lUI);
    float tY = -(tUI + bUI) / (tUI - bUI);
    mat3 projection_UI{{sX,  0.f, 0.f},
                       {0.f, sY,  0.f},
                       {tX,  tY,  1.f}};

    if (m_state == 1) {
        for (auto &entity: m_entities) entity->draw(projection_2D);
        m_ui.draw(projection_UI);

    } else {
        m_states[m_state]->draw(projection_2D);
        m_ui.draw(projection_UI);
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


vec2 World::getPlayerScreenPosition() const {
    return {
            getPlayerPosition().x - m_camera.getLeftBoundary(),
            getPlayerPosition().y - m_camera.getTopBoundary()
    };
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

bool World::isMoving() const {
    return  playerMoving;
}

bool World::isShot() const {
    return  is_shot;
}


void World::addPoints(int points) {
    m_points += points;
}

void World::addPlayerLife() {
    getPlayer()->addLife();
}

void World::increaseCityHealth() {
    getBackground()->addHealth();
}

void World::decrementRemainingEnemies() {
    m_remainingEnemiesInWave--;
}

vec2 World::getNearestEnemyPosToPlayer() const {
    // Default position to return if there are currently no enemies in the world
    vec2 nearestPos = {-1.f, -1.f};
    float closestDistSq = dot(m_size, m_size);

    for (auto &entity : m_entities) {
        // Filter by enemies
        if (entity->getFaction() == Entity::FACTION::ALIEN && entity->isDamageable()) {
            float diffX = entity->getPosition().x - getPlayer()->getPosition().x;
            float diffY = entity->getPosition().y - getPlayer()->getPosition().y;
            vec2 diff = {diffX, diffY};
            float distSq = dot(diff, diff);

            if (distSq < closestDistSq) {
                closestDistSq = distSq;
                nearestPos = entity->getPosition();
            }
        }
    }

    return nearestPos;
}

bool World::isOffScreenEnemyPresentAndNoEnemiesVisible() const {
    bool isAtLeastOneEnemyAlive = false;
    bool noEnemyVisible = true;

    for (auto &entity : m_entities) {
        if (entity->getFaction() == Entity::FACTION::ALIEN && entity->isDamageable()) {
            isAtLeastOneEnemyAlive = true;

            if (m_camera.isEntityInView(*entity)) {
                noEnemyVisible = false;
            }
        }
    }

    return isAtLeastOneEnemyAlive && noEnemyVisible;
}

int World::getPlayerLives() const {
    return getPlayer()->getLives();
}

int World::getWorldHealth() const {
    return getBackground()->getHealth();
}

int World::getWave() const {
    return m_waveNo;
}

int World::getScore() const {
    return m_points;
}

int World::getBestScore() const {
    return m_bestScore;
}

bool World::getInvincibility() {
    return m_invincibility;
}

void World::makeInvincible() {
    m_invincibility = true;
    getPlayer()->setInvincibility(m_invincibility);
    getBackground()->setInvincibility(m_invincibility);
    m_invincibility_countdown = 5000.f;
}

bool World::isPlayerCritical() const {
    return getPlayer()->isCritical();
}


// Private

bool World::initGraphics() {
    return BomberBomb::initGraphics() &&
           NormalBomb::initGraphics() &&
           OneUp::initGraphics() &&
           CityUp::initGraphics() &&
           Shield::initGraphics() &&
           Shooter::initGraphics() &&
           Chaser::initGraphics() &&
           Bomber::initGraphics() &&
           PlayerBullet::initGraphics() &&
           ShooterBullet::initGraphics() &&
           background::initGraphics() &&
           StartScreen::initGraphics() &&
           Info::initGraphics() &&
           GameOver::initGraphics() &&
           HighScore::initGraphics() &&
           shipParticle::initGraphics() &&
           stars::initGraphics();

}

std::shared_ptr<Player> World::getPlayer() const {
    return std::dynamic_pointer_cast<Player>(m_entities.at(1));
}

std::shared_ptr<background> World::getBackground() const {
    return std::dynamic_pointer_cast<background>(m_entities.front());
}

std::shared_ptr<shipParticle> World::getParticle() const {
    return std::dynamic_pointer_cast<shipParticle>(m_entities.front());
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

bool World::initScore() {
    m_scoreFile = fopen(scores_path("score.txt"), "r");
    if (m_scoreFile == nullptr) {
        std::cout << "score file not loaded";
        return false;
    }
    fscanf(m_scoreFile, "%d", &m_bestScore);

    fclose(m_scoreFile);

    return true;
}

void World::advanceWave() {
    MAX_SHOOTERS += 10;
    MAX_CHASER += 5;
    MAX_BOMBER += 5;
    MAX_BOMBS += 5;
    if (POWERUP_DELAY_MS > MIN_POWERUP_DELAY_MS) POWERUP_DELAY_MS -= 1000;

    remainingShootersToSpawn = MAX_SHOOTERS;
    remainingChasersToSpawn = MAX_CHASER;
    remainingBombersToSpawn = MAX_BOMBER;
    remainingNormalBombsToSpawn = MAX_BOMBS;
    m_remainingEnemiesInWave = remainingShootersToSpawn + remainingChasersToSpawn + remainingBombersToSpawn;

    m_waveNo++;
}

// On key callback
void World::onKey(GLFWwindow *, int key, int, int action, int mod) {
    if (key == GLFW_KEY_W) {
        playerMoving = true;
        if (action == GLFW_PRESS) {

            getPlayer()->setFlying(Player::DIRECTION::FORWARD, true);
        } else if (action == GLFW_RELEASE) {
            playerMoving = false;
            getPlayer()->setFlying(Player::DIRECTION::FORWARD, false);
        }
    }

    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            playerMoving = true;
            getPlayer()->setFlying(Player::DIRECTION::BACKWARD, true);
        } else if (action == GLFW_RELEASE) {
            playerMoving = false;
            getPlayer()->setFlying(Player::DIRECTION::BACKWARD, false);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            playerMoving = true;
            getPlayer()->setFlying(Player::DIRECTION::LEFT, true);
        } else if (action == GLFW_RELEASE) {
            playerMoving = false;
            getPlayer()->setFlying(Player::DIRECTION::LEFT, false);
        }
    }

    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            playerMoving = true;
            getPlayer()->setFlying(Player::DIRECTION::RIGHT, true);
        } else if (action == GLFW_RELEASE) {
            playerMoving = false;
            getPlayer()->setFlying(Player::DIRECTION::RIGHT, false);
        }


    }

    // TODO: integrate with game over
    if (key == GLFW_KEY_C) {
        if (action == GLFW_RELEASE) {
            // Update the best score if possible - run this code when
            // we reach game over screen
            if (m_points > m_bestScore) {
                m_bestScore = m_points;
                m_scoreFile = fopen(scores_path("score.txt"), "w+");
                fprintf(m_scoreFile, "%d", m_bestScore);
                fclose(m_scoreFile);
            }
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
            stateStack.push(m_state);
            m_state = 2;
            m_ui.destroy();
            m_ui.init();
        }
    }

    if (key == GLFW_KEY_B && (m_state == 2  || m_state == 4)) {
        if (action == GLFW_PRESS) {
            m_ui.destroy();
            m_state = stateStack.top();
            m_ui.init();
        }
    }

    if (key == GLFW_KEY_H && m_state == 0) {
        if (action == GLFW_PRESS) {
            stateStack.push(m_state);
            m_state = 4;
            m_ui.destroy();
            m_ui.init();
        }
    }

    if (key == GLFW_KEY_P && m_state == 3) {
        if (action == GLFW_PRESS) {
            m_state = 1;
            m_ui.destroy();
            m_ui.init();
            reset();
        }
    } else if (key == GLFW_KEY_P) {
        if (action == GLFW_PRESS) {
            m_state = 1;
            m_ui.destroy();
            m_ui.init();
        }
    }


}




void World::onMouseMove(GLFWwindow *window, double xpos, double ypos) {
    vec2 playerScreenPos = getPlayerScreenPosition();
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
    return m_state;
}

void World::addState(std::shared_ptr<Entity> entity) {
    m_states.emplace_back(entity);
}

void World::reset() {
    m_entities.clear();
    MAX_BOMBERBOMBS = 0;
    MAX_BOMBS = 0;
    MAX_SHOOTERS = 20;
    MAX_CHASER = 0;
    MAX_BOMBER = 0;
    remainingNormalBombsToSpawn = MAX_BOMBS;
    remainingShootersToSpawn = MAX_SHOOTERS;
    remainingChasersToSpawn = MAX_CHASER;
    remainingBombersToSpawn = MAX_BOMBER;
    m_invincibility = false;
    m_remainingEnemiesInWave = remainingShootersToSpawn + remainingChasersToSpawn + remainingBombersToSpawn;
    m_points = 0;
    m_waveNo = 1;
    m_ui.init();
    initGraphics();
    auto bg = std::make_shared<background>(*this);
    bg->init();
    addEntity(bg);
    auto player = std::make_shared<Player>(*this);
    player->init();
    addEntity(player);

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
}



