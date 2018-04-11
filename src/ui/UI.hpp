//
// Created by Shrey Swades Nayak on 2018-04-08.
//

#pragma once


#include <vector>
#include <memory>
#include "../common.hpp"
#include "UIobject.hpp"
#include "playerIcon.hpp"

class World;

class UI {
public:
    UI(vec2 screenSize, World &world);

    bool init();

    void update(float ms);

    void draw(const mat3 &projection);

    vec2 getPlayerScreenPosition() const;

    vec2 getPlayerPosition() const;

    vec2 getNearestEnemyPosToPlayer() const;

    bool isOffScreenEnemyPresentAndNoEnemiesVisible() const;

private:

    bool initGraphics();


private:
    /**
     * All UI objects. The playerIcon will always be the first object, followed by the healthbars.
     */

    std::vector<std::shared_ptr<UIobject>> m_objects;

    vec2 m_screenSize;

    float UIwidth;
    float UIheight;

    World *m_world;

};