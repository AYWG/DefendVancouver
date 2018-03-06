//
// Created by Shrey Swades Nayak on 2018-02-26.
//

#include <iostream>
#include "bomb.hpp"

Texture Bomb::bomb_texture;

using namespace std;

bool Bomb::init(const char *path) {

    //load texture
    if (!bomb_texture.is_valid()) {
        if (!bomb_texture.load_from_file(path)) {
            fprintf(stderr, "Failed to load spritesheet!");
            return false;
        }
    }

    // The position corresponds to the center of the bomb
    float wr = bomb_texture.width * 0.5f;
    float hr = bomb_texture.height * 0.5f;

    vertices[0].position = { -wr, +hr, -0.01f };
    vertices[0].texcoord = { 0.f, 0.33f };
    vertices[1].position = { +wr, +hr, -0.01f };
    vertices[1].texcoord = { 0.33f, 0.33f};
    vertices[2].position = { +wr, -hr, -0.01f };
    vertices[2].texcoord = { 0.33f, 0.f};
    vertices[3].position = { -wr, -hr, -0.01f };
    vertices[3].texcoord = { 0.f, 0.f};

    uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };
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

    isHit = false;
    frameCount = 9;
    b_scale.x = 0.25f;
    b_scale.y = 0.25f;

    return true;

}

void Bomb::draw(const mat3& projection){
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    // Setting shaders
    transform_begin();
    transform_translate(b_position);
    transform_scale(b_scale);
    transform_end();
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bomb_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

bool Bomb::update(float ms){
    if(frameCount != 0){
        frameCount = frameCount - 1;
    }

    switch (frameCount){
        case 9:
            vertices[0].texcoord = {0.f, 0.33f};
            vertices[1].texcoord = {0.33f, 0.33f};
            vertices[2].texcoord = {0.33f, 0.f};
            vertices[3].texcoord = {0.f, 0.f};
        case 8:
            vertices[0].texcoord = {0.33f, 0.33f};
            vertices[1].texcoord = {0.66f, 0.33f};
            vertices[2].texcoord = {0.66f, 0.f};
            vertices[3].texcoord = {0.33f, 0.f};
            break;
        case 7:
            vertices[0].texcoord = {0.66f, 0.33f};
            vertices[1].texcoord = {1.f, 0.33f};
            vertices[2].texcoord = {1.f, 0.f};
            vertices[3].texcoord = {0.66f, 0.f};
            break;
        case 6:
            vertices[0].texcoord = {0.f, 0.66f};
            vertices[1].texcoord = {0.33f, 0.66f};
            vertices[2].texcoord = {0.33f, 0.33f};
            vertices[3].texcoord = {0.f, 0.33f};
            break;
        case 5:
            vertices[0].texcoord = {0.33f, 0.66f};
            vertices[1].texcoord = {0.66f, 0.66f};
            vertices[2].texcoord = {0.66f, 0.33f};
            vertices[3].texcoord = {0.33f, 0.33f};
            break;
        case 4:
            vertices[0].texcoord = {0.66f, 0.66f};
            vertices[1].texcoord = {1.f, 0.66f};
            vertices[2].texcoord = {1.f, 0.33f};
            vertices[3].texcoord = {0.66f, 0.33f};
            break;
        case 3:
            vertices[0].texcoord = {0.f, 1.f};
            vertices[1].texcoord = {0.33f, 1.f};
            vertices[2].texcoord = {0.33f, 0.66f};
            vertices[3].texcoord = {0.f, 0.66f};
            break;
        case 2:
            vertices[0].texcoord = {0.33f, 1.f};
            vertices[1].texcoord = {0.66f, 1.f};
            vertices[2].texcoord = {0.66f, 0.66f};
            vertices[3].texcoord = {0.33f, 0.66f};
            break;
        case 1:
            vertices[0].texcoord = {0.66f, 1.f};
            vertices[1].texcoord = {1.f, 1.f};
            vertices[2].texcoord = {1.f, 0.66f};
            vertices[3].texcoord = {0.66f, 0.66f};
            break;
        default:
            vertices[0].texcoord = {0.f, 0.f};
            vertices[1].texcoord = {0.f, 0.f};
            vertices[2].texcoord = {0.f, 0.f};
            vertices[3].texcoord = {0.f, 0.f};
            break;
    }
    uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };
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

    return true;
}

vec2 Bomb::get_position() const{
    return b_position;
}



void Bomb::set_position(vec2 position){
    b_position = position;
}

void Bomb::animate()
{
    isHit = true;
}

int Bomb::getFrameCount() const {
    return frameCount;
}