//
// Created by gowth on 2018-02-08.
//
#include "player.hpp"

#include <iostream>
#include <algorithm>

int Player::bulletDelayMS = 200;

bool Player::init(vec2 worldSize) {
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
    m_lives = 5;
    m_num_indices = indices.size();
    m_position = {700.f, 500.f};
    m_worldSize = worldSize;
    m_rotation = 0.f;
    m_maxSpeed = 500.f;
    m_isShootingEnabled = false;
    m_timeSinceLastBulletShot = 1000.f;
    m_rng = std::default_random_engine(std::random_device()());
    return true;
}


void Player::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
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
    auto accelerationMagnitude = 1000.f;
    auto deltaXVelocity = netAccelerationDirection.x * accelerationMagnitude * ms / 1000;
    auto deltaYVelocity = netAccelerationDirection.y * accelerationMagnitude * ms / 1000;

    m_velocity = getNewVelocity(m_velocity, {deltaXVelocity, deltaYVelocity});

    auto x_step = m_velocity.x * (ms / 1000);
    auto y_step = m_velocity.y * (ms / 1000);

    auto newXPos = std::min(m_worldSize.x - 50.f, std::max(50.f, m_position.x + x_step));
    auto newYPos = std::min(m_worldSize.y - 50.f, std::max(50.f, m_position.y + y_step));
    m_position = {newXPos, newYPos};

    m_nextBulletSpawn = std::max(0.f, m_nextBulletSpawn - ms);
    m_timeSinceLastBulletShot = std::min(1000.f, m_timeSinceLastBulletShot + ms);
    if (m_isShootingEnabled) {
        shoot();
    }
}


void Player::draw(const mat3 &projection) {
    transform_begin();

    transform_translate({m_position.x, m_position.y});
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
    GLint light_up_uloc = glGetUniformLocation(effect.program, "light_up");

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

    // !!! Salmon Color

    float color[] = {1.f, 1.f, 1.f};

    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);

    int light_up = 0;


    light_up = 0;
    glUniform1iv(light_up_uloc, 1, &light_up);



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

void Player::enableShooting(bool isShooting) {
    m_isShootingEnabled = isShooting;
}

unsigned int Player::getMass() const {
    return 100;
}

void Player::shoot() {
    if (m_nextBulletSpawn == 0.f) {
        if (auto newPlayerBullet = PlayerBullet::spawn()) {
            m_bullets.emplace_back(newPlayerBullet);
        }
        auto newPlayerBulletPtr = m_bullets.back();
        newPlayerBulletPtr->setPosition(m_position);

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

        newPlayerBulletPtr->setVelocity(bulletVelocityRelativeToWorld);
        newPlayerBulletPtr->setRotation(atanf(bulletVelocityRelativeToWorld.y / bulletVelocityRelativeToWorld.x) + 3.1415f / 2);
        m_nextBulletSpawn = Player::bulletDelayMS;
        m_timeSinceLastBulletShot = 0.f;
    }
}

std::vector<std::shared_ptr<PlayerBullet>> &Player::getBullets() {
    return m_bullets;
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

int Player::getLives() {
    return m_lives;
}

void Player::addLives() {
    m_lives++;
}

void Player::hit() {
    m_lives--;
}

vec2 Player::getBoundingBox() const {
    return vec2();
}

bool Player::collisionCheck(Shooter shooter) {
    auto d = magnitude({m_position.x - shooter.getPosition().x, m_position.y - shooter.getPosition().y});
    auto shooterRadius = std::max(shooter.getBoundingBox().x, shooter.getBoundingBox().y) / 2;
    auto playerRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < shooterRadius + playerRadius;
}

bool Player::collisionCheck(Bomber &bomber) {
    return false;
}

bool Player::collisionCheck(ShooterBullet sb) {
    return false;
}

bool Player::collisionCheck(Chaser chaser) {
    auto d = magnitude({m_position.x - chaser.getPosition().x, m_position.y - chaser.getPosition().y});
    auto chaserRadius = std::max(chaser.getBoundingBox().x, chaser.getBoundingBox().y) / 2;
    auto playerRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < chaserRadius + playerRadius;
}

vec2 Player::getBoundingBox() {
    Vertex min;
    Vertex max;
    for (auto &vertex : vertices){
        if (vertex.position.x > max.position.x && vertex.position.y > max.position.y){
            max = vertex;
        }
        if (vertex.position.x < min.position.x && vertex.position.y < min.position.y){
            min = vertex;
        }
    }

    return {std::fabs(m_scale.x) * (max.position.x - min.position.x), std::fabs(m_scale.x) * (max.position.x - min.position.x)};
}

bool Player::collisionCheck(BomberBomb &bomb) {
    auto d = magnitude({m_position.x - bomb.getPosition().x, m_position.y - bomb.getPosition().y});
    auto bombRadius = std::max(bomb.getBoundingBox().x, bomb.getBoundingBox().y) / 2;
    auto playerRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < bombRadius + playerRadius;
}

bool Player::collisionCheck(OneUp &oneup) {
    auto d = magnitude({m_position.x - oneup.getPosition().x, m_position.y - oneup.getPosition().y});
    auto oneupRadius = std::max(oneup.getBoundingBox().x, oneup.getBoundingBox().y) / 2;
    auto playerRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < oneupRadius + playerRadius;
}

bool Player::collisionCheck(Shield &shield) {
    auto d = magnitude({m_position.x - shield.getPosition().x, m_position.y - shield.getPosition().y});
    auto shieldRadius = std::max(shield.getBoundingBox().x, shield.getBoundingBox().y) / 2;
    auto playerRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < shieldRadius + playerRadius;
}




bool Player::collisionCheck(Chaser chaser) {
    auto d = magnitude({m_position.x - chaser.getPosition().x, m_position.y - chaser.getPosition().y});
    auto chaserRadius = std::max(chaser.getBoundingBox().x, chaser.getBoundingBox().y) / 2;
    auto playerRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < chaserRadius + playerRadius;
}

vec2 Player::getBoundingBox() {
    Vertex min;
    Vertex max;
    for (auto &vertex : vertices){
        if (vertex.position.x > max.position.x && vertex.position.y > max.position.y){
            max = vertex;
        }
        if (vertex.position.x < min.position.x && vertex.position.y < min.position.y){
            min = vertex;
        }
    }

    return {std::fabs(m_scale.x) * (max.position.x - min.position.x), std::fabs(m_scale.x) * (max.position.x - min.position.x)};
}
