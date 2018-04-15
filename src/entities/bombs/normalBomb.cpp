//
// Created by Shrey Swades Nayak on 2018-02-26.
//

#include <iostream>
#include <cmath>
#include "normalBomb.hpp"
#include "../../world.hpp"

Graphics NormalBomb::gfx;

NormalBomb::NormalBomb(World &world) : Bomb(world) {}

bool NormalBomb::initGraphics() {
    //load texture
    if (!gfx.texture.is_valid()) {
        if (!gfx.texture.load_from_file(textures_path("normal_bomb.png"))) {
            fprintf(stderr, "Failed to load spritesheet!");
            return false;
        }
    }

    // The position corresponds to the center of the bomb
    float wr = gfx.texture.width * 0.5f;
    float hr = gfx.texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-wr, +hr, -0.01f};
    vertices[0].texcoord = {0.f, 1.f / 3};
    vertices[1].position = {+wr, +hr, -0.01f};
    vertices[1].texcoord = {1.f / 3, 1.f / 3};
    vertices[2].position = {+wr, -hr, -0.01f};
    vertices[2].texcoord = {1.f / 3, 0.f};
    vertices[3].position = {-wr, -hr, -0.01f};
    vertices[3].texcoord = {0.f, 0.f};

    uint16_t indices[] = {0, 3, 1, 1, 3, 2};
    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &gfx.mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gfx.mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &gfx.mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &gfx.mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    return gfx.effect.load_from_file(shader_path("spritesheet.vs.glsl"), shader_path("spritesheet.fs.glsl"));
}

std::shared_ptr<NormalBomb> NormalBomb::spawn(World &world) {
    auto bomb = std::make_shared<NormalBomb>(world);
    if (bomb->init()) {
        world.addEntity(bomb);
        return bomb;
    }
    fprintf(stderr, "Failed to spawn normal bomb");
    return nullptr;
}

bool NormalBomb::init() {
    m_isHit = false;
    frameCount = 0;
    frameWidth = 1.f / 3;
    frameHeight = 1.f / 3;
    frameNumber = 3;
    m_invulnerabilityCountdown = 1500;
    m_scale.x = 0.25f;
    m_scale.y = 0.25f;
    m_initCountdown = 400.f;
    m_explosionCountdown = 50.f;

    return true;
}

void NormalBomb::destroy() {
    glDeleteBuffers(1, &gfx.mesh.vbo);
    glDeleteBuffers(1, &gfx.mesh.ibo);
    glDeleteVertexArrays(1, &gfx.mesh.vao);

    gfx.effect.release();
}

void NormalBomb::draw(const mat3 &projection) {

    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    // Setting shaders
    transform_begin();
    transform_translate(m_position);
    transform_scale(m_scale);
    transform_end();
    glUseProgram(gfx.effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(gfx.effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(gfx.effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(gfx.effect.program, "projection");
    GLint frameCount_uloc = glGetUniformLocation(gfx.effect.program, "frameCount");
    GLint frameNumber_uloc = glGetUniformLocation(gfx.effect.program, "frameNumber");
    GLint frameWidth_uloc = glGetUniformLocation(gfx.effect.program, "frameWidth");
    GLint frameHeight_uloc = glGetUniformLocation(gfx.effect.program, "frameHeight");

    // Setting vertices and indices
    glBindVertexArray(gfx.mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, gfx.mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(gfx.effect.program, "in_position");
    GLint in_texcoord_loc = glGetAttribLocation(gfx.effect.program, "in_texcoord");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_texcoord_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) 0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gfx.texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);
    glUniform1iv(frameCount_uloc, 1, &frameCount);
    glUniform1iv(frameNumber_uloc, 1, &frameNumber);
    glUniform1fv(frameWidth_uloc, 1, &frameWidth);
    glUniform1fv(frameHeight_uloc, 1, &frameHeight);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void NormalBomb::update(float ms) {
    if (m_initCountdown > 0.f) {
        m_initCountdown -= ms;
    }

    if (!m_isHit && m_initCountdown < 0.f) {
        frameCount = 1;
    }

    if (m_invulnerabilityCountdown > 0) {
        m_invulnerabilityCountdown -= ms;
    }

    if (m_isHit) {
        m_scale = {0.75, 0.75};
        m_explosionCountdown -= ms;
    }

    if (m_explosionCountdown < 0.f) {
        m_explosionCountdown = 50.f;
        frameCount++;
    }

    if (frameCount == 9) {
        m_isDead = true;
    }
}

Region NormalBomb::getBoundingBox() const {
    vec2 boxSize = {std::fabs(m_scale.x) * gfx.texture.width, std::fabs(m_scale.y) * gfx.texture.height};
    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};

    return {boxOrigin, boxSize};
}

std::string NormalBomb::getName() const {
    return "NormalBomb";
}

NormalBomb::FACTION NormalBomb::getFaction() const {
    return FACTION::NONE;
}

void NormalBomb::onCollision(Entity &other) {
    if (!isInvulnerable() && other.isDamageable()) {
        other.takeDamage();
        explode();
    }
}

bool NormalBomb::isDamageable() const {
    return !isInvulnerable();
}
