//
// Created by Andy on 2018-02-08.
//

#pragma once

#include "common.hpp"

class Camera {
public:
    /**
     * Moves the camera to newPos
     * @param newPos
     */
    void move(vec2 newPos);

    /**
     * Returns the position of the camera's origin relative to the world - this is the top left of the camera
     * @return
     */
    vec2 getOrigin();


private:
    // The dimensions of the camera - should be the same as the window
    vec2 m_size;

    // The position of the camera in the world - this corresponds to a point in the center of the camera
    vec2 m_pos;
};


