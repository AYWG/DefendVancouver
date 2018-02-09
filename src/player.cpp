//
// Created by gowth on 2018-02-08.
//

#include <vector>
#include <iostream>
#include "player.hpp"
#include "common.hpp"

Texture player::player_texture;


using namespace std;

bool player::init() {
    //load texture
    if(!player_texture.is_valid())
    {
        if(!player_texture.load_from_file(textures_path("player.png")))
        {
            fprintf(stderr, "Failed to load player texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = player_texture.width * 0.5f;
    float hr = player_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = { -wr, +hr, -0.01f };
    vertices[0].texcoord = { 0.f, 1.f };
    vertices[1].position = { +wr, +hr, -0.01f };
    vertices[1].texcoord = { 1.f, 1.f,  };
    vertices[2].position = { +wr, -hr, -0.01f };
    vertices[2].texcoord = { 1.f, 0.f };
    vertices[3].position = { -wr, -hr, -0.01f };
    vertices[3].texcoord = { 0.f, 0.f };

    // counterclockwise as it's the default opengl front winding direction
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

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture
    m_scale.x = -0.4f;
    m_scale.y = 0.4f;
    m_rotation = 0.f;
    m_position.x = 50;
    m_position.y = 50;
    set_rotation(1.571f);






    return true;

}

void player::update(float ms){
    const float PLAYER_SPEED = 200.f;
    float step = PLAYER_SPEED * (ms / 1000);

    if (m_move_up) {

        move({ (float)0.0, (float)-(step) });
    }
    if (m_move_dwn) {
        move({ (float)0.0, (float)(step) });
    }
    if (m_move_rht) {
        //m_isMove = true;
        move({ (float)(step) , (float)0.0 });
    }
    if (m_move_lft) {
        move({ (float)-(step) , (float)0.0 });
    }
}

// Renders the salmon
void player::draw(const mat3& projection){
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform_begin();
    transform_translate(m_position);
    transform_rotate(m_rotation);
    transform_scale(m_scale);
    transform_end();

    // Setting shaders
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
    glBindTexture(GL_TEXTURE_2D, player_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 player::get_position()const{
    return m_position;
}


void player::set_rotation(float radians)
{
    m_rotation = radians;
}

void player::move(vec2 off)
{
    m_position.x += off.x; m_position.y += off.y;
}

bool player::is_move()const
{
    return m_isMove;
}

void player::isMoveUp(bool moveUp)
{
    if (moveUp) {
        m_move_up = true;
    }
    else {
        m_move_up = false;
    }
}

void player::isMoveDwn(bool moveDwn)
{
    if (moveDwn) {
        m_move_dwn = true;
    }
    else {
        m_move_dwn = false;
    }
}

void player::isMoveRht(bool moveRht)
{
    if (moveRht) {
        m_move_rht = true;
    }
    else {
        m_move_rht = false;
    }
}

void player::isMoveLft(bool moveLft)
{
    if (moveLft) {
        m_move_lft = true;
    }
    else {
        m_move_lft = false;
    }
}
