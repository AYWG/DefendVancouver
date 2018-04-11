//
// Created by Andy on 2018-04-11.
//

#include <cmath>
#include "enemyIndicator.hpp"
#include "UI.hpp"

Graphics EnemyIndicator::gfx;

EnemyIndicator::EnemyIndicator(UI &ui) : UIobject(ui) {}

bool EnemyIndicator::initGraphics() {
    //Load texture
    if (!gfx.texture.is_valid()) {
        if (!gfx.texture.load_from_file(textures_path("enemyIndicator.png"))) {
            fprintf(stderr, "Failed to load enemy indicator texture!");
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

bool EnemyIndicator::init() {
    m_position = {400.f, 75.f};
    m_scale.x = 0.2f;
    m_scale.y = 0.2f;

    return true;
}

void EnemyIndicator::destroy() {
    glDeleteBuffers(1, &gfx.mesh.vbo);
    glDeleteBuffers(1, &gfx.mesh.ibo);
    glDeleteVertexArrays(1, &gfx.mesh.vao);

    gfx.effect.release();
}

void EnemyIndicator::draw(const mat3 &projection) {
    if (!m_isHidden) {
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
}

void EnemyIndicator::update(float ms) {
    if (m_ui->isOffScreenEnemyPresentAndNoEnemiesVisible()) {
        m_isHidden = false;

        float xDiffFromPlayer = m_ui->getNearestEnemyPosToPlayer().x - m_ui->getPlayerPosition().x;
        float yDiffFromPlayer = m_ui->getNearestEnemyPosToPlayer().y - m_ui->getPlayerPosition().y;
        float angle = atanf(yDiffFromPlayer / xDiffFromPlayer);
        if (xDiffFromPlayer < 0.f) angle += 3.1415f;
        m_rotation = angle;

        // move indicator to appropriate position on edge of screen
        m_position = m_ui->getPlayerScreenPosition();

        vec2 dirVec = {xDiffFromPlayer, yDiffFromPlayer};
        float length = magnitude(dirVec);
        vec2 unitDirVec = {dirVec.x / length, dirVec.y / length};

        while (isFullyInsideUI()) {
            m_position.x += unitDirVec.x;
            m_position.y += unitDirVec.y;
        }

        // bring the indicator slightly away from the edge of the screen so that it's still fully shown
        // when pointing upwards
        m_position.x -= 10 * unitDirVec.x;
        m_position.y -= 10 * unitDirVec.y;

    } else {
        // hide the indicator
        m_isHidden = true;
    }
}

Region EnemyIndicator::getBoundingBox() const {
    vec2 boxSize = {std::fabs(m_scale.x) * gfx.texture.width,
                    std::fabs(m_scale.y) * gfx.texture.height};
    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};

    return {boxOrigin, boxSize};
}

// Private

bool EnemyIndicator::isFullyInsideUI() const {
    Region box = getBoundingBox();

    bool isWithinLeftBoundary = box.origin.x >= 0;
    bool isWithinRightBoundary = box.origin.x + box.size.x <= m_ui->getScreenSize().x;
    bool isWithinTopBoundary = box.origin.y >= 0;
    bool isWithinBottomBoundary = box.origin.y + box.size.y <= m_ui->getScreenSize().y;

    return isWithinLeftBoundary && isWithinRightBoundary && isWithinTopBoundary && isWithinBottomBoundary;
}