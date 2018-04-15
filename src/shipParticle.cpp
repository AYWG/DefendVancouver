//
// Created by gowth on 2018-04-10.
//

#include <memory>
#include "shipParticle.hpp"
#include "world.hpp"
#include <vector>
#include <iostream>
#include <algorithm>


Graphics shipParticle::gfx;

shipParticle::shipParticle(World &world) : Entity(world) {};

//////////////PARTICLE INIT/////////////
struct Particle{
    vec2 pos;
    vec2 speed ;
    float life;
    float angle;

};
const int MaxParticles = 200;
Particle ParticlesContainer[MaxParticles];;
int ParticlesCount = 0;



/////////////////////////////////////////




bool shipParticle::initGraphics() {
    //load texture
    if (!gfx.texture.is_valid()) {
        if (!gfx.texture.load_from_file(textures_path("particle.png"))) {
            fprintf(stderr, "Failed to load background texture!");
            return false;
        }
    }
    if (gfx.effect.load_from_file(shader_path("particleTexture.vs.glsl"), shader_path("textured.fs.glsl"))) {
    // The position corresponds to the center of the texture
    float wr = gfx.texture.width * 0.5f;
    float hr = gfx.texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-wr, -hr, -0.01f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+wr, -hr, -0.01f};
    vertices[1].texcoord = {1.f, 1.f,};
    vertices[2].position = {+wr, +hr, -0.01f};
    vertices[2].texcoord = {1.f, 0.f};
    vertices[3].position = {-wr, +hr, -0.01f};
    vertices[3].texcoord = {0.f, 0.f};

    uint16_t indices[] = {2, 3, 1, 1, 3, 0};
    // Clearing errors
    gl_flush_errors();

         GLint in_position_loc = glGetAttribLocation(gfx.effect.program, "in_position");
        GLint in_wrldposition_loc = glGetAttribLocation(gfx.effect.program, "in_world_pos");




        // Vertex Array (Container for Vertex + Index buffer)
        glGenVertexArrays(1, &gfx.mesh.vao);
        glBindVertexArray(gfx.mesh.vao);

        // Index Buffer creation
        glGenBuffers(1, &gfx.mesh.ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.mesh.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);


        // Vertex Buffer creation
        glGenBuffers(1, &gfx.mesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, gfx.mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);


        //DYNAMIC Vertex Buffer creation
        glGenBuffers(1, &gfx.particleVBO.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, gfx.particleVBO.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertex) * 2, vertices, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(in_wrldposition_loc);
        glVertexAttribPointer(in_wrldposition_loc, 4, GL_FLOAT, GL_FALSE, sizeof(particleVertex), (void *) 0);

        gl_flush_errors();
    }


    return true;


}

bool shipParticle::init() {

    for (auto& p : ParticlesContainer){
        p.pos.x = m_position.x;
        p.pos.y = m_position.y;

        p.life = 5000;


        float vel = (rand() % 1500 - 750) / 10000.0f;//((float)rand() / RAND_MAX);
        float vel2 = (rand() % 1500 - 750) / 10000.0f;//((float)rand() / RAND_MAX);

        p.speed.x = vel ;
        p.speed.y = vel2;


        normalize(p.speed);

    }
    



    alphaBlend = true;



    return true;
}

void shipParticle::draw(const mat3 &projection) {
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    // Setting shaders
    transform_begin();
    //transform_translate(m_position);
    transform_scale(m_scale);
    transform_rotate(m_rotation);
    transform_end();
    glUseProgram(gfx.effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND);
   // glBlendFunc(GL_ONE, GL_ONE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint color_uloc = glGetUniformLocation(gfx.effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(gfx.effect.program, "projection");

    // Setting vertices and indices
    glBindVertexArray(gfx.mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, gfx.mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(gfx.effect.program, "in_position");
    GLint in_wrldposition_loc = glGetAttribLocation(gfx.effect.program, "in_world_pos");
    GLint in_texcoord_loc = glGetAttribLocation(gfx.effect.program, "in_texcoord");

    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_wrldposition_loc);
    glEnableVertexAttribArray(in_texcoord_loc);

    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) 0);
    glVertexAttribPointer(in_texcoord_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) sizeof(vec3));

    glVertexAttribDivisor(in_position_loc, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(in_texcoord_loc, 0);

    glVertexAttribDivisor(in_wrldposition_loc, 1); // particles vertices : always reuse the same 4 vertices -> 0
    //glVertexAttribDivisor(in_texcoord_loc, 1); // positions : one per quad (its center) -> 1
    glBindBuffer(GL_ARRAY_BUFFER, gfx.particleVBO.vbo);

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gfx.texture.id);

    // Setting uniform values to the currently bound program
    //glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);



   //DRAW PARTICLE!
    glDrawElementsInstanced(GL_TRIANGLES ,6,GL_UNSIGNED_SHORT, nullptr, pos_buf.size());



    glDisableVertexAttribArray(in_position_loc);
    glDisableVertexAttribArray(in_texcoord_loc);
    glDisableVertexAttribArray(in_wrldposition_loc);



    gl_has_errors();

}

void shipParticle::update(float ms) {

    pos_buf.clear();

    int ParticlesCount = 0;



    for (auto& p : ParticlesContainer) {

        if (p.life > 0.0f) {
            p.life -= 50;
            if (p.life > 0.0f) {
                p.pos.x += p.speed.x * 40  ;
                p.pos.y += p.speed.y * 40;
                //p.angle = m_rotation;

                m_position = p.pos;

                pos_buf.push_back(p.pos);

                p.life = 5000;

            }
            ParticlesCount++;

        }
    }


    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBindBuffer(GL_ARRAY_BUFFER, gfx.particleVBO.vbo);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * sizeof(GLfloat) * 2,NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 2, pos_buf.data());

}


void shipParticle::setPosition(vec2 position) {
    m_position = position;
}




std::string shipParticle::getName() const {
    return "particle";
}

void shipParticle::destroy() {

}

Region shipParticle::getBoundingBox() const {

}

void shipParticle::onCollision(Entity &other) {
    // do nothing
}

void shipParticle::takeDamage() {
    // do nothing
}

bool shipParticle::isDamageable() const {
    return false;
}

shipParticle::FACTION shipParticle::getFaction() const {
    return FACTION::NONE;
}




