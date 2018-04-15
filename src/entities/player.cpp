//
// Created by gowth on 2018-02-08.
//
#include "player.hpp"
#include "../world.hpp"
#include <iostream>
#include <algorithm>

int Player::bulletDelayMS = 200;

Player::Player(World &world) : Entity(world), m_nextBulletSpawn(0.f) {}

bool Player::init() {
    std::vector<uint16_t> indices;

    // Reads the salmon mesh from a file, which contains a list of vertices and indices
    mesh_file = fopen(mesh_path("player.mesh"), "r");
    if (mesh_file == nullptr) {
        std::cout << "mesh file not loaded";
        return false;
    }

    // Reading vertices and colors
    char test[100];
    int num_vertices;
    //  fread(test, 100, 1, mesh_file);
    fscanf(mesh_file, "%du\n", &num_vertices);

    for (auto i = 0; i < num_vertices; ++i) {
        float x, y, z;
        float _u[3]; // unused
        int r, g, b;
        fscanf(mesh_file, "%f %f %f %f %f %f %d %d %d\n", &x, &y, &z, _u, _u + 1, _u + 2, &r, &g, &b);
        Vertex vertex;
        vertex.position = {x, y, -z};
        vertex.color = {(float) r / 255, (float) g / 255, (float) b / 255};
        vertices.push_back(vertex);
    }

    // Reading associated indices
    int num_indices;
    fscanf(mesh_file, "%du\n", &num_indices);
    for (auto i = 0; i < num_indices; ++i) {
        int idx[3];
        fscanf(mesh_file, "%d %d %d\n", idx, idx + 1, idx + 2);
        indices.push_back((uint16_t) idx[0]);
        indices.push_back((uint16_t) idx[1]);
        indices.push_back((uint16_t) idx[2]);
    }

    // Done reading
    fclose(mesh_file);

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("colored.vs.glsl"), shader_path("colored.fs.glsl")))
        return false;

    // Setting initial values
    m_scale.x = 200.f;
    m_scale.y = 200.f;
    m_num_indices = indices.size();
    m_lives = 5;
    m_position = {1500.f, 1000.f};
    m_rotation = 0.f;
    m_maxSpeed = 500.f;
    m_isShootingEnabled = false;
    m_timeSinceLastBulletShot = 1000.f;
    m_isHit = false;
    gameOver = false;
    transparency = 1.f;
    countdown = 0.f;
    for (int i = 0; i < NUM_DIRECTIONS; i++) m_isFlying[i] = false;
    m_rng = std::default_random_engine(std::random_device()());
    return true;
}


void Player::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteProgram(effect.program);
}

void Player::update(float ms) {
    auto drag = 1.05f;
    m_velocity.x /= drag;
    m_velocity.y /= drag;

    // determine net acceleration vector based on m_isFlying
    // get direction vector for each flag that is set, then add up the direction vectors, then normalize
    vec2 netAccelerationDirection = {0.f, 0.f};
    for (int dir = LEFT; dir < NUM_DIRECTIONS; dir++) {
        if (m_isFlying[dir]) {
            auto orientation = getMovementOrientation(static_cast<DIRECTION>(dir));
            vec2 direction = {cosf(orientation), sinf(orientation)};
            netAccelerationDirection.x += direction.x;
            netAccelerationDirection.y += direction.y;
        }
    }
    netAccelerationDirection = normalize(netAccelerationDirection);
    auto accelerationMagnitude = 2300.f;
    auto deltaXVelocity = netAccelerationDirection.x * accelerationMagnitude * ms / 1000;
    auto deltaYVelocity = netAccelerationDirection.y * accelerationMagnitude * ms / 1000;

    m_velocity = getNewVelocity(m_velocity, {deltaXVelocity, deltaYVelocity});

    auto x_step = m_velocity.x * (ms / 1000);
    auto y_step = m_velocity.y * (ms / 1000);

    auto newXPos = std::min(m_world->getSize().x - 50.f, std::max(50.f, m_position.x + x_step));
    auto newYPos = std::min(m_world->getSize().y - 50.f, std::max(50.f, m_position.y + y_step));
    m_position = {newXPos, newYPos};

    m_nextBulletSpawn = std::max(0.f, m_nextBulletSpawn - ms);
    m_timeSinceLastBulletShot = std::min(1000.f, m_timeSinceLastBulletShot + ms);

    if (countdown > 0.f) {
        countdown -= ms;
    } else {
        m_isHit = false;
    }

    if (m_isHit && m_lives > 0) {
        transparency = sin(ms * 1000);
    } else if (m_isHit && m_lives <= 0) {
        transparency = countdown / 1500;
    } else {
        transparency = 1.f;
    }

    if (m_lives <= 0 & countdown < 0.f) {
        gameOver = true;
    }

}


void Player::draw(const mat3 &projection) {
    transform_begin();

    transform_translate(m_position);
    transform_rotate(m_rotation);
    transform_scale(m_scale);

    transform_end();

    // Setting shaders
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Getting uniform locations
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");
    GLint transparency_uloc = glGetUniformLocation(effect.program, "transparency");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_color_loc = glGetAttribLocation(effect.program, "in_color");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_color_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(vec3));

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);

    float color[] = {1.f, 1.f, 1.f};

    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);


    glUniform1fv(transparency_uloc, 1, &transparency);

    // Drawing!
    glDrawElements(GL_TRIANGLES, (GLsizei) m_num_indices, GL_UNSIGNED_SHORT, nullptr);
}

void Player::move(vec2 off) {
    m_position.x += off.x;
    m_position.y += off.y;
}

void Player::setFlying(DIRECTION dir, bool isFlying) {
    m_isFlying[dir] = isFlying;
}

bool Player::isShooting() const {
    return m_isShootingEnabled;
}

void Player::enableShooting(bool isShooting) {
    m_isShootingEnabled = isShooting;
}

unsigned int Player::getMass() const {
    return 100;
}

void Player::shoot() {
    if (m_nextBulletSpawn == 0.f) {
        auto newPlayerBullet = PlayerBullet::spawn(*m_world);
        newPlayerBullet->setPosition(m_position);

        float bulletInitialSpeed = 1000.f;
        float bulletAngleRelativeToPlayer = m_rotation + 3.1415f / 2.f +
                                            3.1415f / 12.f * (1000 - m_timeSinceLastBulletShot) / 1000 * m_dist(m_rng);
        vec2 bulletDirectionRelativeToPlayer = {cosf(bulletAngleRelativeToPlayer), sinf(bulletAngleRelativeToPlayer)};

        // bullet's initial velocity (in the world)
        // is sum of player's current velocity and the initial velocity relative to the player
        vec2 bulletVelocityRelativeToPlayer = {bulletInitialSpeed * bulletDirectionRelativeToPlayer.x,
                                               bulletInitialSpeed * bulletDirectionRelativeToPlayer.y};

        vec2 bulletVelocityRelativeToWorld = {m_velocity.x + bulletVelocityRelativeToPlayer.x,
                                              m_velocity.y + bulletVelocityRelativeToPlayer.y};

        newPlayerBullet->setVelocity(bulletVelocityRelativeToWorld);
        newPlayerBullet->setRotation(
                atanf(bulletVelocityRelativeToWorld.y / bulletVelocityRelativeToWorld.x) + 3.1415f / 2);
        m_nextBulletSpawn = Player::bulletDelayMS;
        m_timeSinceLastBulletShot = 0.f;
    }
}

int Player::getLives() {
    return m_lives;
}

void Player::addLife() {
    m_lives++;
}

Region Player::getBoundingBox() const {
    Vertex min = vertices.front();
    Vertex max = vertices.front();
    for (auto &vertex : vertices) {
        if (vertex.position.x > max.position.x && vertex.position.y > max.position.y) {
            max = vertex;
        }
        if (vertex.position.x < min.position.x && vertex.position.y < min.position.y) {
            min = vertex;
        }
    }

    vec2 boxSize = {std::fabs(m_scale.x) * (max.position.x - min.position.x),
                    std::fabs(m_scale.y) * (max.position.y - min.position.y)};
    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};

    return {boxOrigin, boxSize};
}


std::string Player::getName() const {
    return "Player";
}

void Player::onCollision(Entity &entity) {

}

void Player::takeDamage() {
    m_isHit = true;
    countdown = 1500.f;
    m_lives--;
}

bool Player::isDamageable() const {
    return !m_isInvincible && !m_isHit;
}

Player::FACTION Player::getFaction() const {
    return FACTION::HUMAN;
}

void Player::setInvincibility(bool isInvincible) {
    m_isInvincible = isInvincible;
}

bool Player::isCritical() const {
    return m_lives == 1;
}

// Private methods

float Player::getMovementOrientation(DIRECTION dir) {
    return m_rotation + dir * 3.1415f / 2;
}

vec2 Player::getNewVelocity(vec2 oldVelocity, vec2 delta) {
    vec2 newVelocity = {(oldVelocity.x + delta.x), (oldVelocity.y + delta.y)};
    float newMagnitude = std::min(magnitude(newVelocity), m_maxSpeed);
    vec2 newDirection = normalize(newVelocity);

    return {newDirection.x * newMagnitude, newDirection.y * newMagnitude};
}



