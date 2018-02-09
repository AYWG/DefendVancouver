//
// Created by gowth on 2018-02-08.
//

#include <vector>
#include "player.hpp"
#include "common.hpp"

Texture player::player_texture;



bool player::init() {
    //load texture
    if(!player_texture.is_valid())
    {
        if(!player_texture.load_from_file(textures_path("fish.png")))
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

    transform_translate({m_position.x, m_position.y});

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
