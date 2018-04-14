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
#include "entities/powerups/oneUp.hpp"
#include "entities/powerups/cityUp.hpp"
#include "entities/powerups/shield.hpp"
#include "ui/UI.hpp"

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
    void update(float ms);

    // Renders our scene
    void draw();

    // Should the game be over ?
    bool is_over() const;

    vec2 getPlayerPosition() const;

    vec2 getPlayerScreenPosition() const;

    std::vector<vec2> getBombPositions() const;

    vec2 getCityPosition() const;

    void addEntity(std::shared_ptr<Entity> entity);

    vec2 getSize() const;

    bool isEntityInView(const Entity &entity) const;

    void addPoints(int points);

    void addPlayerLife();

    void increaseCityHealth();

    void decrementTotalEnemies();
    
    vec2 getNearestEnemyPosToPlayer() const;

    bool isOffScreenEnemyPresentAndNoEnemiesVisible() const;

    int getPlayerLives() const;

    int getWorldHealth() const;

    int getWave() const;

    int getScore() const;

    int getBestScore() const;

    bool getInvincibility();

    void makeInvincible();


    float bulletAngleRelativeToPlayer;
    vec2 bulletDirectionRelativeToPlayer;
    bool isGraphCreated = false;
    int grid[ROW][COL];
    float  width;
    float  height;

private:
    bool initGraphics();

    std::shared_ptr<Player> getPlayer() const;
    std::shared_ptr<background> getBackground() const;


    void playerBounce(const NormalBomb &bomb);

    bool initScore();

    // !!! INPUT CALLBACK FUNCTIONS
    void onKey(GLFWwindow *, int key, int, int action, int mod);

    void onMouseMove(GLFWwindow *window, double xpos, double ypos);

    void onMouseClick(GLFWwindow *window, int button, int action, int mod);



private:
    // Window handle
    GLFWwindow *m_window;

    FILE *m_scoreFile;

    int m_bestScore;

    // Number of fish eaten by the salmon, displayed in the window title
    unsigned int m_points;

    /**
     * All entities in the world. The background will always be the first entity, followed by the player.
     */
    std::vector<std::shared_ptr<Entity>> m_entities;


    float m_next_shooter_spawn;
    float m_next_chaser_spawn;
    float m_next_bomber_spawn;
    float m_next_nbomb_spawn;
    float m_next_bbomb_spawn;
    float m_next_oneup_spawn;
    float m_next_cityup_spawn;
    float m_next_shield_spawn;

    // C++ rng
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist; // default 0..1
    static Texture world_texture;

    // World size
    vec2 m_size;
    Camera m_camera;
    UI m_ui;

    QuadTreeNode m_quad;
    
    int totalEnemies;
    int waveNo;

    bool m_invincibility;
    float m_invincibility_countdown;
    
};
