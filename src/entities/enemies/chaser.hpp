//
// Created by Shrey Swades Nayak on 2018-02-08.
//
#pragma once

#include "../../common.hpp"

#include "enemy.hpp"
#include "../../ai/chaserAI.hpp"


using namespace std;

#define  ROW 100
#define  COL 300

//shortcut for int, int pair type
typedef pair<int, int> Pair;

//creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;


class Chaser : public Enemy, public Renderable {


    static Texture chaserTexture;
    static int maxNumberOfBullets;
    static int bulletDelayMS;

public:

    explicit Chaser(ChaserAI &ai);

    //Neccesary param
    struct cell {
        //0 <= i <= ROW-1 & 0 <= j <= COL-1
        int parent_i, parent_j;
        //f = g+h
        double f, g, h;
    };


    bool init() override;


    void destroy() override;

    void update(World *world, float ms) override;

    void draw(const mat3 &projection) override;

    vec2 getBoundingBox() const override;

    bool isValid(int row, int col);

    bool isUnBlocked(int grid[][COL], int row, int col);

    bool isDestination(int row, int col, Pair dest);

    double calculateHValue(int row, int col, Pair dest);

    void tracePath(cell cellDetails[][COL], Pair dest);

    void aStarSearch(int grid[][COL], Pair src, Pair dest);

    vec2 move(vec2 off);

    void attack(float ms) override;

    unsigned int getMass() const override;

private:
    ChaserAI m_ai;
    float m_nextChaserBulletSpawn;
    float m_rotation;
    vec2 curr_pos;
    vec2 curr_scale;
    float curr_rotation;

    //    vec2 getBoundingBox()const;


};