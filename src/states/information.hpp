//
// Created by Tanwin Nun on 2018-04-12.
//

#ifndef DEFENDVANCOUVER_INFORMATION_HPP
#define DEFENDVANCOUVER_INFORMATION_HPP


#include "state.hpp"

class Info: public State {
public:
    static Texture info_texture;

    static bool initTexture();

    Info(World &world);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    void setPosition(vec2 pos);

    Region getBoundingBox() const override;

    std::string getName() const override;


};


#endif //DEFENDVANCOUVER_INFORMATION_HPP
