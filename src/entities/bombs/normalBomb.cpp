//
// Created by Shrey Swades Nayak on 2018-02-26.
//

#include <iostream>
#include <cmath>
#include "normalBomb.hpp"
#include "../../world.hpp"

Texture NormalBomb::bomb_texture;

NormalBomb::NormalBomb(World &world) : Entity(world) {}

NormalBomb::~NormalBomb() {
    destroy();
}

bool NormalBomb::initTexture() {
    //load texture
    if (!bomb_texture.is_valid()) {
        if (!bomb_texture.load_from_file(textures_path("normal_bomb.png"))) {
            fprintf(stderr, "Failed to load spritesheet!");
            return false;
        }
    }
    return true;
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
    // The position corresponds to the center of the bomb
    float wr = bomb_texture.width * 0.5f;
    float hr = bomb_texture.height * 0.5f;

    frameWidth = 1.f/3;
    frameHeight = 1.f/3;

    vertices[0].position = {-wr, +hr, -0.01f};
    vertices[0].texcoord = {0.f, frameHeight};
    vertices[1].position = {+wr, +hr, -0.01f};
    vertices[1].texcoord = {frameWidth, frameHeight};
    vertices[2].position = {+wr, -hr, -0.01f};
    vertices[2].texcoord = {frameWidth, 0.f};
    vertices[3].position = {-wr, -hr, -0.01f};
    vertices[3].texcoord = {0.f, 0.f};

    uint16_t indices[] = {0, 3, 1, 1, 3, 2};
    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("spritesheet.vs.glsl"), shader_path("spritesheet.fs.glsl")))
        return false;

    isHit = false;
    frameCount = 0;
    m_scale.x = 0.25f;
    m_scale.y = 0.25f;

    return true;

}

void NormalBomb::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    effect.release();
}

void NormalBomb::draw(const mat3 &projection) {

    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    // Setting shaders
    transform_begin();
    transform_translate(m_position);
    transform_scale(m_scale);
    transform_end();
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");
    GLint frameCount_uloc = glGetUniformLocation(effect.program, "frameCount");
    GLint frameWidth_uloc = glGetUniformLocation(effect.program, "frameWidth");
    GLint frameHeight_uloc = glGetUniformLocation(effect.program, "frameHeight");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_texcoord_loc = glGetAttribLocation(effect.program, "in_texcoord");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_texcoord_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) 0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bomb_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);
    glUniform1iv(frameCount_uloc, 1, &frameCount);
    glUniform1fv(frameWidth_uloc, 1, &frameWidth);
    glUniform1fv(frameHeight_uloc, 1, &frameHeight);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void NormalBomb::update(float ms) {

    if(!isHit){
        frameCount = 1;
    } else {
        frameCount++;
    }

    if (frameCount == 9) {
        m_isDead = true;
    }
}

// Returns the local bounding coordinates scaled by the current size of the bomb
//vec2 NormalBomb::getBoundingBox() const {
//     fabs is to avoid negative scale due to the facing direction
//    return {std::fabs(m_scale.x) * (bomb_texture.width / 3), std::fabs(m_scale.y) * (bomb_texture.height / 3)};
//}

Region NormalBomb::getBoundingBox() const {
    vec2 boxSize = {std::fabs(m_scale.x) * bomb_texture.width, std::fabs(m_scale.y) * bomb_texture.height};
    vec2 boxOrigin = { m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};

    return {boxOrigin, boxSize};
}

void NormalBomb::animate() {
    isHit = true;
}

int NormalBomb::getFrameCount() const {
    return frameCount;
}

std::string NormalBomb::getName() const {
    return "NormalBomb";
}
