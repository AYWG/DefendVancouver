#pragma once

// internal
#include "common.hpp"
#include "entities/player.hpp"
#include "entities/enemies/shooter.hpp"
#include "entities/enemies/bomber.hpp"
#include "entities/enemies/chaser.hpp"
#include "background.hpp"
#include "camera.hpp"
#include "entities/bombs/normalBomb.hpp"
#include "entities/bombs/bomberBomb.hpp"
#include "entities/bullets/playerBullet.hpp"
#include "explosion.hpp"
#include "collisions/quadTreeNode.hpp"

// stlib
#include <vector>
#include <list>
#include <random>


// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class World {
public:
    World();

    ~World();

    // Creates a window, sets up events and begins the game
    bool init(vec2 screenSize, vec2 worldSize);

    // Releases all associated resources
    void destroy();

    // Steps the game ahead by ms milliseconds
    bool update(float ms);

    bool elapsedUpdate(float ms);

    // Renders our scene
    void draw();

    // Should the game be over ?
    bool is_over() const;

    vec2 getPlayerPosition() const;

    std::vector<vec2> getBombPositions() const;

    vec2 getCityPosition() const;

    vec2 playerCenter;
    float bulletAngleRelativeToPlayer;
    vec2 bulletDirectionRelativeToPlayer;
    bool isGraphCreated = false;

private:
    bool initTextures();

    void playerBounce(const NormalBomb &bomb);

    bool bomberOnScreen(Bomber &bomber);

    // !!! INPUT CALLBACK FUNCTIONS
    void onKey(GLFWwindow *, int key, int, int action, int mod);

    void onMouseMove(GLFWwindow *window, double xpos, double ypos);

    void onMouseClick(GLFWwindow *window, int button, int action, int mod);

private:
    // Window handle
    GLFWwindow *m_window;

    // Number of fish eaten by the salmon, displayed in the window title
    unsigned int m_points;

    background m_background;
    // Game entities
    std::shared_ptr<Player> m_player;
    std::list<std::shared_ptr<Chaser>> m_chasers;
    std::list<std::shared_ptr<Shooter>> m_shooters;
    std::list<std::shared_ptr<Bomber>> m_bombers;
    std::list<std::shared_ptr<NormalBomb>> m_normalBombs;
    std::list<std::shared_ptr<BomberBomb>> m_bomberBombs;

    float m_next_shooter_spawn;
    float m_next_chaser_spawn;
    float m_next_bomber_spawn;
    float m_next_nbomb_spawn;
    float m_next_bbomb_spawn;

    // C++ rng
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist; // default 0..1
    static Texture world_texture;

    // World size
    vec2 m_size;

    Camera m_camera;

    QuadTreeNode m_quad;
};
