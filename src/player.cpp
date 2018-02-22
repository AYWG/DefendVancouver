//
// Created by gowth on 2018-02-08.
//

#include <vector>
#include <iostream>
#include "player.hpp"
#include "common.hpp"

Texture Player::player_texture;


using namespace std;

bool Player::init() {
    //load texture
    if (!player_texture.is_valid()) {
        if (!player_texture.load_from_file(textures_path("player.png"))) {
            fprintf(stderr, "Failed to load player texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = player_texture.width * 0.5f;
    float hr = player_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-wr, +hr, -0.01f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+wr, +hr, -0.01f};
    vertices[1].texcoord = {1.f, 1.f,};
    vertices[2].position = {+wr, -hr, -0.01f};
    vertices[2].texcoord = {1.f, 0.f};
    vertices[3].position = {-wr, -hr, -0.01f};
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

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture
    m_scale.x = -0.2f;
    m_scale.y = 0.2f;
    m_rotation = 0.f;
    m_position.x = 123;
    m_position.y = 82;
    m_max_speed = 200.f;
    set_rotation(1.571f);

    return true;

}

void Player::update(float ms) {

    float x_step = (m_velocity[RIGHT] - m_velocity[LEFT]) * (ms / 1000);
    float y_step = (m_velocity[DOWN] - m_velocity[UP]) * (ms / 1000);
    move({x_step, y_step});

    for (int dir = 0; dir < NUM_DIRECTIONS; dir++) {
        if (!m_is_flying[dir] && m_velocity[dir] > 0) {
            m_velocity[dir] = std::max(0.f, m_velocity[dir] - 5.f);
        }
    }

    get_position();


}

void Player::draw(const mat3 &projection) {
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
    glBindTexture(GL_TEXTURE_2D, player_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 Player::get_position() const {
    return m_position;
}


void Player::set_rotation(float radians) {
    m_rotation = radians;
}

void Player::move(vec2 off) {
    m_position.x += off.x;
    m_position.y += off.y;
}

bool Player::is_move() const {
    return m_isMove;
}


void Player::set_velocity(float velocity, DIRECTION dir) {
    m_velocity[dir] = velocity;
}


void Player::set_flying(bool is_flying, DIRECTION dir) {
    m_is_flying[dir] = is_flying;
}

float Player::get_max_speed() const {
    return m_max_speed;
}




/*

#include "player.hpp"

// stlib
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

bool Player::init()
{
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    // Reads the salmon mesh from a file, which contains a list of vertices and indices
    FILE* mesh_file = fopen(mesh_path("salmon.mesh"), "r");
    if (mesh_file == nullptr) {
        std::cout<<"mesh file not loaded";
        return false;
    }

    // Reading vertices and colors
    size_t num_vertices;
    fscanf(mesh_file, "%zu\n", &num_vertices);
    for (size_t i = 0; i < num_vertices; ++i)
    {
        float x, y, z;
        float _u[3]; // unused
        int r, g, b;
        fscanf(mesh_file, "%f %f %f %f %f %f %d %d %d\n", &x, &y, &z, _u, _u+1, _u+2, &r, &g, &b);
        Vertex vertex;
        vertex.position = { x, y, -z };
        vertex.color = { (float)r / 255, (float)g / 255, (float)b / 255 };
        vertices.push_back(vertex);
    }

    // Reading associated indices
    size_t num_indices;
    fscanf(mesh_file, "%zu\n", &num_indices);
    for (size_t i = 0; i < num_indices; ++i)
    {
        int idx[3];
        fscanf(mesh_file, "%d %d %d\n", idx, idx + 1, idx + 2);
        indices.push_back((uint16_t)idx[0]);
        indices.push_back((uint16_t)idx[1]);
        indices.push_back((uint16_t)idx[2]);
    }

    // Done reading
    fclose(mesh_file);

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("colored.vs.glsl"), shader_path("colored.fs.glsl")))
        return false;

    // Setting initial values
    m_scale.x = 150.f;
    m_scale.y = 150.f;

    m_num_indices = indices.size();
    m_position = { 50.f, 100.f };
    m_rotation = 0.f;

    m_isMove = false;

    return true;
}

// Releases all graphics resources
void Player::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

// Called on each frame by World::update()
void Player::update(float ms)
{
    float x_step = (m_velocity[RIGHT] - m_velocity[LEFT]) * (ms / 1000);
    float y_step = (m_velocity[DOWN] - m_velocity[UP]) * (ms / 1000);
    move({x_step, y_step});

    for (int dir = 0; dir < NUM_DIRECTIONS; dir++) {
        if (!m_is_flying[dir] && m_velocity[dir] > 0) {
            m_velocity[dir] = std::max(0.f, m_velocity[dir] - 5.f);
        }
    }

    get_position();
}

void Player::draw(const mat3& projection)
{
    transform_begin();

    transform_translate({ m_position.x, m_position.y});
    transform_rotate(-m_rotation);
    transform_scale(m_scale);

    transform_end();

    // Setting shaders
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Getting uniform locations
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");
    GLint light_up_uloc = glGetUniformLocation(effect.program, "light_up");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_color_loc = glGetAttribLocation(effect.program, "in_color");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_color_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);

    // !!! Salmon Color

    float color[] = { 1.f, 1.f, 1.f };

    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // HERE TO SET THE CORRECTLY LIGHT UP THE SALMON IF HE HAS EATEN RECENTLY
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    int light_up = 0;


    light_up = 0;
    glUniform1iv(light_up_uloc, 1, &light_up);



    // Drawing!
    glDrawElements(GL_TRIANGLES,(GLsizei)m_num_indices, GL_UNSIGNED_SHORT, nullptr);
}





vec2 Player::get_position() const {
    return m_position;
}


void Player::set_rotation(float radians) {
    m_rotation = radians;
}

void Player::move(vec2 off) {
    m_position.x += off.x;
    m_position.y += off.y;
}

bool Player::is_move() const {
    return m_isMove;
}


void Player::set_velocity(float velocity, DIRECTION dir) {
    m_velocity[dir] = velocity;
}


void Player::set_flying(bool is_flying, DIRECTION dir) {
    m_is_flying[dir] = is_flying;
}

float Player::get_max_speed() const {
    return m_max_speed;

}
*/
