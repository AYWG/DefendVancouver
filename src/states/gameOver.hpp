//
// Created by Tanwin Nun on 2018-04-11.
//

#ifndef DEFENDVANCOUVER_GAMEOVER_HPP
#define DEFENDVANCOUVER_GAMEOVER_HPP


#include "state.hpp"

class GameOver: public State {
public:
    static Texture over_texture;

    static bool initTexture();

    GameOver(World &world);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    Region getBoundingBox() const override;

    std::string getName() const override;


};

#endif //DEFENDVANCOUVER_GAMEOVER_HPP
