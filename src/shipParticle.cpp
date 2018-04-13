//
// Created by gowth on 2018-04-10.
//

#include <memory>
#include "shipParticle.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

Graphics shipParticle::gfx;

shipParticle::shipParticle(World &world) : Entity(world) {};

//////////////PARTICLE INIT/////////////
struct Particle{
    vec2 pos;
    vec2 speed ;
    float life = 10000;

};
const int MaxParticles = 100;
Particle ParticlesContainer[MaxParticles];;
int ParticlesCount = 0;


static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];


/////////////////////////////////////////

/*static void APIENTRY openglCallbackFunction(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam
){
    (void)source; (void)type; (void)id;
    (void)severity; (void)length; (void)userParam;
    fprintf(stderr, "%s\n", message);
    if (severity==GL_DEBUG_SEVERITY_HIGH) {
        fprintf(stderr, "Aborting...\n");
        abort();
    }

}*/




bool shipParticle::initGraphics() {
    //load texture
    if (!gfx.texture.is_valid()) {
        if (!gfx.texture.load_from_file(textures_path("particle.png"))) {
            fprintf(stderr, "Failed to load background texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = gfx.texture.width * 0.5f;
    float hr = gfx.texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-wr, +hr, -0.01f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+wr, +hr, -0.01f};
    vertices[1].texcoord = {1.f, 1.f,};
    vertices[2].position = {+wr, -hr, -0.01f};
    vertices[2].texcoord = {1.f, 0.f};
    vertices[3].position = {-wr, -hr, -0.01f};
    vertices[3].texcoord = {0.f, 0.f};

    uint16_t indices[] = {0, 3, 1, 1, 3, 2};
    // Clearing errors
    gl_flush_errors();

    if (gfx.effect.load_from_file(shader_path("particleTexture.vs.glsl"), shader_path("textured.fs.glsl"))) {



        // GLint in_position_loc = glGetAttribLocation(gfx.effect.program, "in_position");
        GLint in_wrldposition_loc = glGetAttribLocation(gfx.effect.program, "in_world_pos");
        GLint in_texcoord_loc = glGetAttribLocation(gfx.effect.program, "in_texcoord");



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

        gl_has_errors();
        //DYNAMIC Vertex Buffer creation
        glGenBuffers(1, &gfx.particleVBO.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, gfx.particleVBO.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(in_wrldposition_loc);
        glEnableVertexAttribArray(in_texcoord_loc);
        glVertexAttribPointer(in_wrldposition_loc, 4, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) 0);
        glVertexAttribPointer(in_texcoord_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) sizeof(vec3));
        gl_flush_errors();
    }
// These functions are specific to glDrawArrays*Instanced*.





    // Loading shaders

    return true;


}

bool shipParticle::init() {
    m_position.x = 500;
    m_position.y = 500;
    m_scale.x = 0.05f;
    m_scale.y = 0.05f;

    vec2 maindir = {0.0f, 10.0f};
    float spread = 1.5f;

    for(int i=0; i<MaxParticles; i++){
        Particle& p = ParticlesContainer[i]; // shortcut
        p.pos = m_position;

        rndmDir = {
                (rand()%90000 - 1000.0f)/1000.0f,
                (rand()%90000 - 1000.0f)/1000.0f
        };

        float vel = ((float)rand() / RAND_MAX);
        float vel2 = ((float)rand() / RAND_MAX);

        p.speed.x = vel ;
        p.speed.y = vel2 ;
    }

    std::cout<<ParticlesContainer[5].speed.y<<", "<<ParticlesContainer[23].speed.y;


    return true;
}

void shipParticle::draw(const mat3 &projection) {
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

    // Setting vertices and indices
    glBindVertexArray(gfx.mesh.vao);
    //glBindBuffer(GL_ARRAY_BUFFER, gfx.mesh.vbo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.mesh.ibo);



    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(gfx.effect.program, "in_position");
    GLint in_wrldposition_loc = glGetAttribLocation(gfx.effect.program, "in_world_pos");
    GLint in_texcoord_loc = glGetAttribLocation(gfx.effect.program, "in_texcoord");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_texcoord_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) 0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) sizeof(vec3));

    glVertexAttribDivisor(in_position_loc, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(in_texcoord_loc, 0);

    glVertexAttribDivisor(in_wrldposition_loc, 1); // particles vertices : always reuse the same 4 vertices -> 0
   // glVertexAttribDivisor(in_texcoord_loc, 1); // positions : one per quad (its center) -> 1


    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gfx.texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);



   //DRAW PARTICLE!


    glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,6,pos_buf.size());


    glDisableVertexAttribArray(in_position_loc);
    glDisableVertexAttribArray(in_texcoord_loc);

    gl_has_errors();

    // Drawing!
}

void shipParticle::update(float ms) {

    pos_buf.clear();

    int ParticlesCount = 0;
   // for(auto &p : ParticlesContainer){
   /* for(int i=0; i<MaxParticles; i++){
        Particle& p = ParticlesContainer[i]; // shortcut

        if (p.life > 0.0f) {
            p.life -= 20;

            if(p.life > 0) {

                *//*if (i%2 == 0) {*//*
                    p.pos.x += p.speed.x;
                    p.pos.y += p.speed.y;
                *//*} else if (i%2 != 0){
                    p.speed.x -= 1.f * ms;
                    p.speed.y -= -0.21f * ms;
                    p.pos.x -= p.speed.x * 0.25f * (ms / 500);
                    p.pos.y -= p.speed.y * 0.25f * (ms / 500);
                }
*//*


                m_position = p.pos;

                pos_buf.push_back(p.pos);


            }
            ParticlesCount++;
        }
    }*/

    Particle& p = ParticlesContainer[1];
    pos_buf.push_back(p.pos);





    glBindBuffer(GL_ARRAY_BUFFER, gfx.particleVBO.vbo);
    //glBufferData(GL_ARRAY_BUFFER, MaxParticles * sizeof(GLfloat) * 4,NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 1 * sizeof(GLfloat) * 2, pos_buf.data());


}





// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int shipParticle::FindUnusedParticle() {

    for(int i=LastUsedParticle; i<MaxParticles; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    return 0;
}

/*void shipParticle::particleGenerator() {
// Generate 10 new particule each millisecond,
// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
// newparticles will be huge and the next frame even longer.
    int newparticles = (int)(delta * 10000.0);
    if (newparticles > (int)(0.016f*10000.0))
        newparticles = (int)(0.016f*10000.0);
}*/



std::string shipParticle::getName() const {
    return "particle";
}

void shipParticle::destroy() {

}

Region shipParticle::getBoundingBox() const {

}

