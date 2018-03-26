//
// Created by Shrey Swades Nayak on 2018-03-26.
//

#ifndef DEFENDVANCOUVER_SHIELD_HPP
#define DEFENDVANCOUVER_SHIELD_HPP

#pragma once

#include <memory>
#include "../entity.hpp"
#include "../../common.hpp"

class Shield : public Entity, public Renderable {

    static Texture shieldTexture;

public:

    static bool initTexture();

    static std::shared_ptr<Shield> spawn();

    ~Shield() override;

    bool init();

    void destroy() override;

    void draw(const mat3 &projection) override;

    bool update(float ms);

    vec2 getBoundingBox() const;

private:

    TexturedVertex vertices[4];
};


#endif //DEFENDVANCOUVER_SHIELD_HPP
