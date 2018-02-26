#pragma once

// internal
#include "pbullet.hpp"
#include "common.hpp"
#include "player.hpp"
#include "enemies/shooter.hpp"
#include "enemies/bomber.hpp"
#include "enemies/chaser.hpp"
#include "background.hpp"
#include "camera.hpp"

// stlib
#include <vector>
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

    // Renders our scene
    void draw();

    // Should the game be over ?
    bool is_over() const;




    vec2 playerCenter;
    vec2 mousePos;
    vec2 aimDir;
    vec2 baimDir;
    vec2 aimDirNorm;
	bool is_shot = false;
    bool is_shoted = false;
    vec2 mouseAimDir;
    //static vec2 shotDir;

private:

    //spawn shooter
    bool spawnShooter();

    bool spawn_playerBullet();

    vec2 get_mousePos(vec2 mousePos);

    // !!! INPUT CALLBACK FUNCTIONS
    void onKey(GLFWwindow *, int key, int, int action, int mod);

    void onMouseMove(GLFWwindow *window, double xpos, double ypos);

    vec2 const mousePosition();


private:
    // Window handle
    GLFWwindow *m_window;

    // Number of fish eaten by the salmon, displayed in the window title
    unsigned int m_points;

    background m_background;
    // Game entities
    Player m_player;

    std::vector<pBullet> m_pbullet;
    std::vector<pBullet> m_shotBullet;


    pBullet m_plbullet;

    std::vector<Shooter> m_shooters;



    float m_next_shooter_spawn;
    float m_current_speed;
    bool m_is_advanced_mode;
    float  m_next_pbullet_spawn;

    // C++ rng
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist; // default 0..1
    static Texture world_texture;


    vec2 m_size;
    vec2 afterShot;

    Camera m_camera;
};
