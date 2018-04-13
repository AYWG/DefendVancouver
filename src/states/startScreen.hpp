//
// Created by Tanwin Nun on 2018-04-11.
//

#ifndef DEFENDVANCOUVER_STARTSCREEN_HPP
#define DEFENDVANCOUVER_STARTSCREEN_HPP


#include "state.hpp"

class StartScreen: public State {
public:
    static Texture start_texture;

    static bool initTexture();

    StartScreen(World &world);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    void setPosition(vec2 pos);

    Region getBoundingBox() const override;

    std::string getName() const override;


};


#endif //DEFENDVANCOUVER_STARTSCREEN_HPP
