//
// Created by Shrey Swades Nayak on 2018-03-26.
//

#include <iostream>
#include "OneUp.hpp"
#include "../../world.hpp"

Texture OneUp::oneupTexture;

OneUp::OneUp(World &world) : Entity(world) {}

OneUp::~OneUp() {
    destroy();
}

bool OneUp::initTexture() {
    //load texture
    if (!oneupTexture.is_valid()) {
        if (!oneupTexture.load_from_file(textures_path("1up.png"))) {
            fprintf(stderr, "Failed to load texture!");
            return false;
        }
    }
    return true;
}

std::shared_ptr<OneUp> OneUp::spawn(World &world) {
    auto oneup = std::make_shared<OneUp>(world);
    if (oneup->init()) {
        world.addEntity(oneup);
        return oneup;
    }
    fprintf(stderr, "Failed to spawn one up");
    return nullptr;
}

bool OneUp::init() {
    //center of texture
    float width = oneupTexture.width * 0.5f;
    float height = oneupTexture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-width, +height, -0.01f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+width, +height, -0.01f};
    vertices[1].texcoord = {1.f, 1.f};
    vertices[2].position = {+width, -height, -0.01f};
    vertices[2].texcoord = {1.f, 0.f};
    vertices[3].position = {-width, -height, -0.01f};
    vertices[3].texcoord = {0.f, 0.f};

    // counterclockwise as it's the default opengl front winding direction
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
    if (!effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    //m_scale.x = 0.2f;
    //m_scale.y = 0.4f;

    return true;
}

void OneUp::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    effect.release();
}

void OneUp::draw(const mat3 &projection) {
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
    glBindTexture(GL_TEXTURE_2D, oneupTexture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void OneUp::update(float ms) {
    const float SPEED = 100.f;
    float step = SPEED * (ms / 1000);
    m_position.y += step;

    if (m_position.y > m_world->getSize().y) {
        m_isDead = true;
    }
}

Region OneUp::getBoundingBox() const {
    vec2 boxSize = {std::fabs(m_scale.x) * oneupTexture.width,
                    std::fabs(m_scale.y) * oneupTexture.height};
    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};

    return {boxOrigin, boxSize};
}

std::string OneUp::getName() const {
    return "OneUp";
}
