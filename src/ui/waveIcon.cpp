//
// Created by Shrey Swades Nayak on 2018-04-11.
//

#include "waveIcon.hpp"
#include "UI.hpp"

Graphics waveIcon::gfx;

waveIcon::waveIcon(UI &ui) : UIobject(ui) {}

waveIcon::~waveIcon() {
    destroy();
}

bool waveIcon::initGraphics() {
//load texture
    if (!gfx.texture.is_valid()) {
        if (!gfx.texture.load_from_file(textures_path("waveIcon.png"))) {
            fprintf(stderr, "Failed to load spritesheet!");
            return false;
        }
    }

    // The position corresponds to the center of the bomb
    float wr = gfx.texture.width * 0.5f;
    float hr = gfx.texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-wr, +hr, -0.01f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+wr, +hr, -0.01f};
    vertices[1].texcoord = {1.f/2, 1.f};
    vertices[2].position = {+wr, -hr, -0.01f};
    vertices[2].texcoord = {1.f/2, 0.f};
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

bool waveIcon::init() {
    frameWidth = 1.f/2;
    frameCount = 0;
    frameNumber = 2;
    m_scale.x = 0.5f;
    m_scale.y = 1.0f;
    countdown = 1500.f;
    start = false;

    vec2 lifeScale = {0.035f, 0.35f};
    vec2 UIsize = m_ui->getScreenSize();
    auto firstDigit = std::make_shared<Digit>(*m_ui);
    if (firstDigit->init()) {
        firstDigit->setPosition({UIsize.x-85.f,80.f});
        firstDigit->setScale(lifeScale);
        m_digits.emplace_back(firstDigit);
    }
    auto secondDigit = std::make_shared<Digit>(*m_ui);
    if (secondDigit->init()) {
        secondDigit->setPosition({firstDigit->getPosition().x+20.f, firstDigit->getPosition().y});
        secondDigit->setScale(lifeScale);
        m_digits.emplace_back(secondDigit);
    }

    return true;

}

void waveIcon::destroy() {
    glDeleteBuffers(1, &gfx.mesh.vbo);
    glDeleteBuffers(1, &gfx.mesh.ibo);
    glDeleteVertexArrays(1, &gfx.mesh.vao);

    gfx.effect.release();
}

void waveIcon::draw(const mat3 &projection) {
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

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    for (auto &m_digit : m_digits) {
        m_digit->draw(projection);
    }
}

void waveIcon::update(float ms) {
    waveCount = m_ui->getWave();

    for(int i = 1; i>=0; i--){
        m_digits[i]->setDigit(waveCount % 10);
        waveCount /= 10;
    }


    if(countdown > 0.f) {
        countdown -= ms;
    } else{
        start = true;
    }
    if(start){
        countdown = 1500;
        start = false;
        frameCount++;
    }
    if(frameCount>2) {
        frameCount = 0;
    }


}