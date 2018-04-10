//
// Created by gowth on 2018-04-10.
//



#include <memory>
#include <../glm/glm/vec4.hpp>


class shipParticle {

public:
    std::unique_ptr<glm::vec4[]> m_pos;
    std::unique_ptr<glm::vec4[]> m_col;
    std::unique_ptr<glm::vec4[]> m_startCol;
    std::unique_ptr<glm::vec4[]> m_endCol;
    std::unique_ptr<glm::vec4[]> m_vel;
    std::unique_ptr<glm::vec4[]> m_acc;
    std::unique_ptr<glm::vec4[]> m_time;
    std::unique_ptr<bool[]>  m_alive;

    size_t m_count{ 0 };
    size_t m_countAlive{ 0 };
public:
    explicit shipParticle(size_t maxCount) { generate(maxCount); }
    ~ParticleData() { }

    shipParticle(const shipParticle &) = delete;
    shipParticle &operator=(const shipParticle &) = delete;

    void generate(size_t maxSize);
    void kill(size_t id);
    void wake(size_t id);
    void swapData(size_t a, size_t b);

};



