//
// Created by gowth on 2018-04-10.
//


#include <vector>
#include "common.hpp"
#include "entities/entity.hpp"



class shipParticle : public Entity, public Renderable  {


public:

    std::vector<vec2> pos_buf;

    static bool initGraphics();

    static std::shared_ptr<shipParticle> spawn(World &world);

    shipParticle(World &world);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    void destroy() override;

    Region getBoundingBox() const override;

    std::string getName() const override;





private:
    static Graphics gfx;
    vec2 m_position;
    vec2 m_scale;




};



