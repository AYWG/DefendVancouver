//
// Created by Shrey Swades Nayak on 2018-03-12.
//

#include "Explosion.hpp"


// Set our static (per class NOT per object!) variable values
const GLfloat Explosion::GRAVITY = 0.05f;
const GLfloat Explosion::baselineYSpeed = -4.0f;
const GLfloat Explosion::maxYSpeed = -4.0f;

// Constructor implementation
Explosion::Explosion() {
    // We call a function to perform the constructor's job here so that we can re-initialise the same Explosion
    // later on without having to destroy the object and recreate it!
    initialise();
}

void Explosion::initialise() {
    // Pick an initial x location and random x/y speeds for each particle making up the Explosion
    // Note: Each particle in the Explosion must have the exact same values for the Explosion to rise as a single point!
    float xLoc = ((float) rand() / (float) RAND_MAX) * 800.0f;
    float xSpeedVal = -2 + ((float) rand() / (float) RAND_MAX) * 4.0f;
    float ySpeedVal = baselineYSpeed + ((float) rand() / (float) RAND_MAX) * maxYSpeed;
    //cout << ySpeedVal << endl;

    // Set initial x/y location and speeds for each particle in the Explosion
    for (int loop = 0; loop < EXPLOSION_PARTICLES; loop++) {
        x[loop] = xLoc;
        y[loop] = 610.0f; // Push the particle location down off the bottom of the screen
        xSpeed[loop] = xSpeedVal;
        ySpeed[loop] = ySpeedVal;
    }

    // Assign a random colour and full alpha (i.e. particle is completely opaque)
    red = ((float) rand() / (float) RAND_MAX);
    green = ((float) rand() / (float) RAND_MAX);
    blue = ((float) rand() / (float) RAND_MAX);
    alpha = 1.0f;

    // Explosion will launch after a random amount of frames between 0 and 400
    framesUntilLaunch = ((int) rand() % 400);

    // Size of the particle (as thrown to glPointSize) - range is 1.0f to 4.0f
    particleSize = 1.0f + ((float) rand() / (float) RAND_MAX) * 3.0f;

    // Flag to keep trackof whether the Explosion has exploded or not
    hasExploded = false;

    //cout << "Initialised a Explosion." << endl;
}

// Function to move a Explosion
void Explosion::move() {
    for (int loop = 0; loop < EXPLOSION_PARTICLES; loop++) {
        // Once the Explosion is ready to launch start moving the particles
        if (framesUntilLaunch <= 0) {
            x[loop] += xSpeed[loop];

            y[loop] += ySpeed[loop];

            ySpeed[loop] += Explosion::GRAVITY;
        }
    }
    framesUntilLaunch--; // Decrease the launch countdown clock!

    // Once a Explosions speed turns positive (i.e. at top of arc) - blow it up!
    if (ySpeed[0] > 0.0f) {
        for (int loop2 = 0; loop2 < EXPLOSION_PARTICLES; loop2++) {
            // Set a random x and y speed beteen -4 and + 4
            xSpeed[loop2] = -4 + (rand() / (float) RAND_MAX) * 8;
            ySpeed[loop2] = -4 + (rand() / (float) RAND_MAX) * 8;
        }

        // Set the Explosion hasExploded flag to true
        hasExploded = true;

        //cout << "Boom!" << endl;
    }
}

// Function to make a Explosion explode
void Explosion::explode() {
    for (int loop = 0; loop < EXPLOSION_PARTICLES; loop++) {
        // Dampen the horizontal speed by 1% per frame
        xSpeed[loop] *= 0.99;

        // Move the particle
        x[loop] += xSpeed[loop];
        y[loop] += ySpeed[loop];

        // Apply gravity to the particle's speed
        ySpeed[loop] += Explosion::GRAVITY;
    }

    // Fade out the particles (alpha is stored per Explosion, not per particle)
    if (alpha > 0.0f) {
        alpha -= 0.01f;
    } else // Once the alpha hits zero, then reset the Explosion
    {

    }
}