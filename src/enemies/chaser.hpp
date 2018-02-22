//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#include "../common.hpp"
#include <bits/stdc++.h>

// Basic alien enemies for the game (grey spaceship)

using  namespace std;

#define  ROW 10
#define  COL 10

//shortcut for int, int pair type
typedef  pair<int, int> Pair;

//creating a shortcut for pair<int, pair<int, int>> type
typedef  pair<double , pair<int, int> > pPair;

class Chaser : public Renderable {

    static Texture chaser_texture;

public:

    //Neccesary param
    struct cell
    {
        //0 <= i <= ROW-1 & 0 <= j <= COL-1
        int parent_i, parent_j;
        //f = g+h
        double f, g, h;
    };

    bool init();

    void destroy();

    void update(float ms);

    void draw(const mat3& projection)override;

    vec2 get_position()const;

    void set_position(vec2 position);

    bool isValid (int row, int col);

    bool isUnBlocked(int grid[][COL], int row, int col);

    bool isDestination(int row, int col, Pair dest);

    double calculateHValue(int row, int col, Pair dest);

    void tracePath(cell cellDetails[][COL], Pair dest);

    void aStarSearch(int grid[][COL], Pair src, Pair dest);
//    vec2 get_bounding_box()const;

private:

    vec2 curr_pos;
    vec2 curr_scale;
    float curr_rotation;

};