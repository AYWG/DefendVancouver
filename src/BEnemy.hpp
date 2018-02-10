//
// Created by gowth on 2018-02-09.
//



#include "common.hpp"

class BEnemy : public  Renderable
{
    static Texture basicEnemy_texture;

public:
    bool init();

    // Renders the salmon
    void draw(const mat3& projection)override;

private:
private:
    vec2 m_position; // Window coordinates
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    float m_rotation; // in radians
    bool m_isMove;
    float m_max_speed;
};