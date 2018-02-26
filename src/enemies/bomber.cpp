//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#include "bomber.hpp"

#include <cmath>

Texture Bomber::bomber_texture;

bool Bomber::init() {

    //Load texture
    if (!bomber_texture.is_valid())
    {
        if (!bomber_texture.load_from_file(textures_path("bomber.png")))
        {
            fprintf(stderr, "Failed to load turtle texture!");
            return false;
        }
    }

    //center of texture
    float width = bomber_texture.width * 0.5f;
    float height = bomber_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = { -width, +height, -0.01f };
    vertices[0].texcoord = { 0.f, 1.f };
    vertices[1].position = { +width, +height, -0.01f };
    vertices[1].texcoord = { 1.f, 1.f };
    vertices[2].position = { +width, -height, -0.01f };
    vertices[2].texcoord = { 1.f, 0.f };
    vertices[3].position = { -width, -height, -0.01f };
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
    m_scale.x = 0.4f;
    m_scale.y = 0.4f;
    m_rotation = 0.f;
    m_pos.x = 475;
    m_pos.y = 300;

    return true;
}

void Bomber::destroy(){

}

void Bomber::update(float ms){

}

void Bomber::draw(const mat3& projection){
    transform_begin();
    transform_translate(m_pos);
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
    glBindTexture(GL_TEXTURE_2D, bomber_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 Bomber::get_position()const{
    return m_pos;
}

void Bomber::set_position(vec2 position){
    m_pos = position;
}

//vec2 get_bounding_box()const;
