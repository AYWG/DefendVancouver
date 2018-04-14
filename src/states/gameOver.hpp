//
// Created by Tanwin Nun on 2018-04-11.
//

#ifndef DEFENDVANCOUVER_GAMEOVER_HPP
#define DEFENDVANCOUVER_GAMEOVER_HPP


#include "state.hpp"

class GameOver: public State {
public:

    static bool initGraphics();

    GameOver(World &world);

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    Region getBoundingBox() const override;

    std::string getName() const override;

private:

    static Graphics gfx;

};

#endif //DEFENDVANCOUVER_GAMEOVER_HPP
