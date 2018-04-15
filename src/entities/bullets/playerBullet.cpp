//
// Created by gowth on 2018-02-09.
//
#include "playerBullet.hpp"
#include "../../world.hpp"


Graphics PlayerBullet::gfx;

PlayerBullet::PlayerBullet(World &world) : Bullet(world) {}

bool PlayerBullet::initGraphics() {
    if (!gfx.texture.is_valid()) {
        if (!gfx.texture.load_from_file(textures_path("playerBullet.png"))) {
            fprintf(stderr, "Failed to load player bullet texture!");
            return false;
        }
    }

    //center of texture
    float width = gfx.texture.width * 0.5f;
    float height = gfx.texture.height * 0.5f;

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
    return gfx.effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl"));
}

std::shared_ptr<PlayerBullet> PlayerBullet::spawn(World &world) {
    auto playerBullet = std::make_shared<PlayerBullet>(world);
    if (playerBullet->init()) {
        world.addEntity(playerBullet);
        return playerBullet;
    }
    fprintf(stderr, "Failed to spawn player bullet");
    return nullptr;
}

bool PlayerBullet::init() {
    m_scale.x = 0.2f;
    m_scale.y = 0.4f;

    return true;
}

void PlayerBullet::destroy() {
    glDeleteBuffers(1, &gfx.mesh.vbo);
    glDeleteBuffers(1, &gfx.mesh.ibo);
    glDeleteVertexArrays(1, &gfx.mesh.vao);

    gfx.effect.release();
}

vec2 PlayerBullet::getPosition() {
    return m_position;
}

void PlayerBullet::update(float ms) {
    float x_step = m_velocity.x * (ms / 1000);
    float y_step = m_velocity.y * (ms / 1000);

    m_position = {m_position.x + x_step, m_position.y + y_step};

    if (!m_world->isEntityInView(*this)) {
        m_isDead = true;
    }
}




void PlayerBullet::draw(const mat3 &projection) {
    transform_begin();
    transform_translate(m_position);
    transform_rotate(m_rotation);
    transform_scale(m_scale);
    transform_end();

    // Setting shaders
    glUseProgram(gfx.effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(gfx.effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(gfx.effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(gfx.effect.program, "projection");

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

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

Region PlayerBullet::getBoundingBox() const {
    vec2 boxSize = {std::fabs(m_scale.x) * gfx.texture.width,
                    std::fabs(m_scale.y) * gfx.texture.height};
    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};

    return {boxOrigin, boxSize};
}

unsigned int PlayerBullet::getMass() const {
    return 10;
}

std::string PlayerBullet::getName() const {
    return "PlayerBullet";
}

PlayerBullet::FACTION PlayerBullet::getFaction() const {
    return FACTION::HUMAN;
}
