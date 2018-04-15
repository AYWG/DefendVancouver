//
// Created by Shrey Swades Nayak on 2018-02-08.
//
#pragma once

#include "../../common.hpp"

#include "enemy.hpp"
#include "../../ai/chaserAI.hpp"



using std::pair;

#define  ROW 100
#define  COL 300

//shortcut for int, int pair type
typedef pair<int, int> Pair;

//creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;


class Chaser : public Enemy, public Renderable {

public:
    static bool initGraphics();

    static std::shared_ptr<Chaser> spawn(World &world);

    Chaser(World &world, ChaserAI &ai);

    //Neccesary param
    struct cell {
        //0 <= i <= ROW-1 & 0 <= j <= COL-1
        int parent_i, parent_j;
        //f = g+h
        double f, g, h;
    };



    bool init() override;

    void destroy() override;

    void update(float ms) override;

    void draw(const mat3 &projection) override;

    Region getBoundingBox() const override;

    bool isValid(int row, int col);

    bool isUnBlocked(int grid[][COL], int row, int col);

    bool isDestination(int row, int col, Pair dest);

    double calculateHValue(int row, int col, Pair dest);

    void tracePath(cell cellDetails[][COL], Pair dest);

    void aStarSearch(int grid[][COL], Pair src, Pair dest);

    vec2 move(vec2 off);

    void attack(float ms) override;

    int getPoints() const override;

    unsigned int getMass() const override;

    std::string getName() const override;

    void aStarGridPlacement();


private:
    static Graphics gfx;
    float countdown;
    float afterCd;
};