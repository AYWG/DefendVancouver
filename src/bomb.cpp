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

    //TexturedVertex vertices[4];

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

    b_scale.x = 1.0f;
    b_scale.y = 1.0f;
    b_position.x = 200;
    b_position.y = 200;


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

void Bomb::update(World *world, float ms){
    float bomb_animation = 1500.f;
    float step = bomb_animation * (ms / 1000);
    mat3 projection_2D = world->getProjectionMatrix();

    vec2 v0 = vertices[0].texcoord; //top left vertex
    vec2 v1 = vertices[1].texcoord; //top right vertex
    vec2 v2 = vertices[2].texcoord; //bottom right vertex
    vec2 v3 = vertices[3].texcoord; //bottom left vertex

    for(int j = 0; j < 3; j++){
        v0.x = 0;
        v1.x = 0.33;
        v2.x = 0.33;
        v3.x = 0;
        if(j == 0){
            v0.y = 0;
            v1.y = 0;
            v2.y = 0.33;
            v3.y = 0.33;
        } else {
            v0.y += 0.33;
            v1.y += 0.33;
            double wy = v2.y + 0.33;
            if(wy < 1){
                v2.y += 0.33;
                v3.y += 0.33;
            } else {
                v2.y = 1;
                v3.y = 1;
            }
        }
        for(int i = 0; i < 3; i++){
            if(i==0){
                continue;
            }
            v0.x += 0.33;
            v3.x += 0.33;
            double wx = v1.x + 0.33;
            if(wx < 1){
                v1.x += 0.33;
                v2.x += 0.33;
            } else {
                v1.x = 1;
                v2.x = 1;
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

            draw(projection_2D);
        }
    }

}

vec2 Bomb::get_position()const{
    return b_position;
}

void Bomb::set_position(vec2 position)
{
    b_position = position;
}