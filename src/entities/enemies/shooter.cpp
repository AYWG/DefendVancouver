//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#include "shooter.hpp"
#include "../../world.hpp"


Texture Shooter::shooterTexture;

int Shooter::bulletDelayMS = 1000;

Shooter::Shooter(ShooterAI &ai) : m_ai(ai), m_nextBulletSpawn(0.f) {}

bool Shooter::init() {

    //Load texture
    if (!shooterTexture.is_valid()) {
        if (!shooterTexture.load_from_file(textures_path("shooter_new.png"))) {
            fprintf(stderr, "Failed to load shooter texture!");
            return false;
        }
    }

    //center of texture
    float width = shooterTexture.width * 0.5f;
    float height = shooterTexture.height * 0.5f;

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

    m_scale.x = 0.4f;
    m_scale.y = 0.4;

    return true;
}

void Shooter::destroy() {

}

void Shooter::update(World *world, float ms) {
    m_ai.doNextAction(world, this, ms);
}

void Shooter::draw(const mat3 &projection) {
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
    glBindTexture(GL_TEXTURE_2D, shooterTexture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

// Returns the local bounding coordinates scaled by the current size of the turtle
vec2 Shooter::getBoundingBox() const {
    // fabs is to avoid negative scale due to the facing direction
    return {std::fabs(m_scale.x) * shooterTexture.width, std::fabs(m_scale.y) * shooterTexture.height};
}



//vec2 get_bounding_box()const;

void Shooter::attack(float ms) {
    m_nextBulletSpawn -= ms;
    if (m_nextBulletSpawn < 0.f) {
        if (auto newShooterBullet = ShooterBullet::spawn()) {
            m_bullets.emplace_back(newShooterBullet);
        }
        auto newShooterBulletPtr = m_bullets.back();
        newShooterBulletPtr->setPosition(m_position);

        float bulletAngle = m_rotation + 3.1415f / 2.f;
        newShooterBulletPtr->setVelocity({cosf(bulletAngle) * 325.0f, sinf(bulletAngle) * 325.0f});
        m_nextBulletSpawn = Shooter::bulletDelayMS;
    }
}

unsigned int Shooter::getMass() const {
    return 100;
}

std::vector<std::shared_ptr<ShooterBullet>> &Shooter::getBullets() {
    return m_bullets;
}
