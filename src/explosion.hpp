//
// Created by Shrey Swades Nayak on 2018-03-12.
//

#ifndef DEFENDVANCOUVER_EXPLOSION_HPP
#define DEFENDVANCOUVER_EXPLOSION_HPP

#include <cstdlib>
#include <GL/gl3w.h> // OpenGL headers

const GLint EXPLOSION_PARTICLES = 70;

class Explosion {
public:
    // Object member properties
    GLfloat x[EXPLOSION_PARTICLES];
    GLfloat y[EXPLOSION_PARTICLES];
    GLfloat xSpeed[EXPLOSION_PARTICLES];
    GLfloat ySpeed[EXPLOSION_PARTICLES];

    GLfloat red;
    GLfloat blue;
    GLfloat green;
    GLfloat alpha;

    GLint framesUntilLaunch;

    GLfloat particleSize;
    GLboolean hasExploded;

    static const GLfloat baselineYSpeed;
    static const GLfloat maxYSpeed;

    static const GLfloat GRAVITY;


    // Object member functions
    Explosion(); // Constructor declaration
    void initialise();

    void move();

    void explode();
};

#endif //DEFENDVANCOUVER_EXPLOSION_HPP
