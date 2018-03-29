//
// Created by Shrey Swades Nayak on 2018-03-26.
//

#pragma once

#include <memory>
#include "../entity.hpp"
#include "../../common.hpp"

class OneUp : public Entity, public Renderable{

    static Texture oneupTexture;

public:

    static bool initTexture();

    static std::shared_ptr<OneUp> spawn();

    ~OneUp() override;

    bool init();

    void destroy() override;

    void draw(const mat3 &projection) override;

    bool update(float ms);

    Region getBoundingBox() const override;

private:

    TexturedVertex vertices[4];


};
