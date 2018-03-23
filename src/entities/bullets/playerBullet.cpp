//
// Created by gowth on 2018-02-09.
//
#include "playerBullet.hpp"
#include <cmath>


Texture PlayerBullet::playerBulletTexture;

std::shared_ptr<PlayerBullet> PlayerBullet::spawn() {
    auto playerBullet = std::make_shared<PlayerBullet>();
    if (playerBullet->init()) {
        return playerBullet;
    }
    fprintf(stderr, "Failed to spawn player bullet");
    return nullptr;
}

bool PlayerBullet::init() {

    //Load texture
    if (!playerBulletTexture.is_valid()) {
        if (!playerBulletTexture.load_from_file(textures_path("playerBullet.png"))) {
            fprintf(stderr, "Failed to load player bullet texture!");
            return false;
        }
    }

    //center of texture
    float width = playerBulletTexture.width * 0.5f;
    float height = playerBulletTexture.height * 0.5f;

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

    m_scale.x = 0.2f;
    m_scale.y = 0.4f;

    return true;
}

void PlayerBullet::update(float ms) {
    float x_step = m_velocity.x * (ms / 1000);
    float y_step = m_velocity.y * (ms / 1000);

    m_position = {m_position.x + x_step, m_position.y + y_step};
}


void PlayerBullet::draw(const mat3 &projection) {
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
    glBindTexture(GL_TEXTURE_2D, playerBulletTexture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 PlayerBullet::getBoundingBox() const {
    return {std::fabs(m_scale.x) * playerBulletTexture.width, std::fabs(m_scale.y) * playerBulletTexture.height};
}

unsigned int PlayerBullet::getMass() const {
    return 10;
}

bool PlayerBullet::collisionCheck(Shooter shooter) {
    auto d = magnitude({m_position.x - shooter.getPosition().x, m_position.y - shooter.getPosition().y});
    auto shooterRadius = std::max(shooter.getBoundingBox().x, shooter.getBoundingBox().y) / 2;
    auto bulletRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < shooterRadius + bulletRadius;
}

bool PlayerBullet::collisionCheck(Chaser chaser) {
    auto d = magnitude({m_position.x - chaser.getPosition().x, m_position.y - chaser.getPosition().y});
    auto chaserRadius = std::max(chaser.getBoundingBox().x, chaser.getBoundingBox().y) / 2;
    auto bulletRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < chaserRadius + bulletRadius;
}

bool PlayerBullet::collisionCheck(Bomber& bomber) {
    auto d = magnitude({m_position.x - bomber.getPosition().x, m_position.y - bomber.getPosition().y});
    auto bomberRadius = std::max(bomber.getBoundingBox().x, bomber.getBoundingBox().y) / 2;
    auto bulletRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < bomberRadius + bulletRadius;
}

bool PlayerBullet::collisionCheck(NormalBomb &bomb) {
    auto d = magnitude({m_position.x - bomb.getPosition().x, m_position.y - bomb.getPosition().y});
    auto bombRadius = std::max(bomb.getBoundingBox().x, bomb.getBoundingBox().y) / 2;
    auto bulletRadius = std::max(getBoundingBox().x, getBoundingBox().y) / 2;
    return d < bombRadius + bulletRadius;
}
