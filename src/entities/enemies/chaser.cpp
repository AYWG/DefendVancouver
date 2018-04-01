//
// Created by Shrey Swades Nayak on 2018-02-08.
//

#include "chaser.hpp"
#include "../../world.hpp"
#include <stack>
#include <cfloat>
#include <set>

using std::stack;
using std::make_pair;
using std::set;

Texture Chaser::chaserTexture;

Chaser::Chaser(World &world, ChaserAI &ai) : Enemy(world, ai) {}

Chaser::~Chaser() {
    destroy();
}

bool Chaser::initTexture() {
    if (!chaserTexture.is_valid()) {
        if (!chaserTexture.load_from_file(textures_path("chaser.png"))) {
            fprintf(stderr, "Failed to load chaser texture!");
            return false;
        }
    }
    return true;
}

std::shared_ptr<Chaser> Chaser::spawn(World &world) {
    auto ai = new ChaserAI;
    auto chaser = std::make_shared<Chaser>(world, *ai);
    if (chaser->init()) {
        world.addEntity(chaser);
        return chaser;
    }
    fprintf(stderr, "Failed to spawn chaser!");
    return nullptr;
}

bool Chaser::init() {
    //center of texture
    float width = chaserTexture.width * 0.5f;
    float height = chaserTexture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-width, +height, -0.01f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+width, +height, -0.01f};
    vertices[1].texcoord = {1.f, 1.f};
    vertices[2].position = {+width, -height, -0.01f};
    vertices[2].texcoord = {1.f, 0.f};
    vertices[3].position = {-width, -height, -0.01f};
    vertices[3].texcoord = {0.f, 0.f};

    // counterclockwise as it's the default opengl front winding direction
    uint16_t indices[] = {0, 3, 1, 1, 3, 2};

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture

    m_scale.x = 0.4f;
    m_scale.y = 0.4f;
    m_rotation = 0.f;
  //  m_position = {500,600};


    return true;
}

void Chaser::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    effect.release();
}


void Chaser::update(float ms){
    float velcity = 30;
    float x_step = velcity * (ms/1000);
    float  y_step = velcity * (ms/1000);

    move({x_step, y_step});
}

void Chaser::draw(const mat3 &projection) {
    transform_begin();
    transform_translate(m_position);
    transform_rotate(m_rotation);
    transform_scale(m_scale);
    transform_end();

    // Setting shaders
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_texcoord_loc = glGetAttribLocation(effect.program, "in_texcoord");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_texcoord_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) 0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, chaserTexture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

Region Chaser::getBoundingBox() const {
    vec2 boxSize = {std::fabs(m_scale.x) * chaserTexture.width, std::fabs(m_scale.y) * chaserTexture.height};
    vec2 boxOrigin = { m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};

    return {boxOrigin, boxSize};
}


vec2 Chaser::move(vec2 off) {
    m_position.x += off.x;
    m_position.y += off.y;

}

void Chaser::attack(float ms) {

}

unsigned int Chaser::getMass() const {
    return 90;
}

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool Chaser::isValid(int row, int col) {
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) &&
           (col >= 0) && (col < COL);
}

// A Utility Function to check whether the given cell is
// blocked or not
bool Chaser::isUnBlocked(int grid[][COL], int row, int col) {
    // Returns true if the cell is not blocked else false
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}

//  check whether destination cell has
// been reached or not
bool Chaser::isDestination(int row, int col, Pair dest) {
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double Chaser::calculateHValue(int row, int col, Pair dest) {
    // Return using the distance formula
    return ((double) sqrt((row - dest.first) * (row - dest.first)
                          + (col - dest.second) * (col - dest.second)));
}

// A Utility Function to trace the path from the source
// to destination
void Chaser::tracePath(cell cellDetails[][COL], Pair dest) {
    // printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        //printf("-> (%d,%d) ", p.first, p.second);
        float speed = 0.25f;
        if (!Path.empty()) {
            pair<int, int> np = Path.top();
            //////////////////////////
            if ((p.first > np.first) && (p.second < np.second)) {
                move({speed, -speed});
            }
            if ((p.first > np.first) && (p.second > np.second)) {
                move({-speed, -speed});
            }
            if ((p.first > np.first) && (p.second == np.second) ) {
                move({0, -speed});
            }
            ///////////////////////////

            if ((p.first < np.first) && (p.second < np.second)) {
                move({speed, speed});
            }
            if ((p.first < np.first) && (p.second > np.second)) {
                move({-speed, speed});
            }
            if ((p.first < np.first) && (p.second == np.second)) {
                move({0, speed});
            }
            ////////////////////////////

            if ((p.first == np.first) && (p.second < np.second)) {
                move({speed, 0});
            }
            if ((p.first == np.first) && (p.second > np.second)) {
                move({-speed, 0});
            }
            if ((p.first == np.first) && (p.second == np.second)) {
                move({0, 0});
            }
            /////////////////////////////

        }
    }
    return;
}

void Chaser::aStarSearch(int grid[][COL], Pair src, Pair dest) {
    //if src us outta range

    if (isValid(src.first, src.second) == false){
        printf("source is invalid");
        move({0, 10});

        return;
    }

    //if dest is outta range
    if (isValid(dest.first, dest.second) == false) {
        //printf("dentination is invalid");
        return;
    }

    //if source or destination is blocked
    if (isUnBlocked(grid, src.first, src.second) == false ||
        isUnBlocked(grid, dest.first, dest.second) == false) {
        // printf ("Source or the destination is blocked\n");
        //    move({-5, 0});
        return;
    }

    // If the destination cell is the same as source
    if (isDestination(src.first, src.second, dest) == true) {
        // printf ("We are already at the destination\n");
        return;
    }

    //create a closed list and init to false
    //0 cells included yet
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));

    //Declare 2D array to hold info of cell
    cell cellDetails[ROW][COL];

    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;

        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    /*
    Create an open list having information as-
    <f, <i, j>>
    where f = g + h,
    and i, j are the row and column index of that cell
    Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    This open list is implenented as a set of pair of pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));

    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the open list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;


        /*
         * North (i-1,j)
         * South (i+1,j)
         * East (i, j+1)
         * West (i, j-1)
         * North-East (i-1, j+1)
         * North-West (i-1, j-1)
         * South-East (i+1, j+1)
         * South-West (i+1, j-1)*/

        //store g,h,f of all the new cells
        double gNew, hNew, fNew;

        /////////NORTH/////////
        if (isValid(i - 1, j) == true) {
            // If the destination cell is the same as the
            // searched cell
            if (isDestination(i - 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                // printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
                // If the seached cell is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i - 1][j] == false &&
                     isUnBlocked(grid, i - 1, j) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i - 1][j].f == FLT_MAX ||
                    cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(fNew,
                                              make_pair(i - 1, j)));

                    // Update the details of this cell
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        /////////SOUTH/////////

        if (isValid(i + 1, j) == true) {
            // If the destination cell is the same as the
            // searched cell
            if (isDestination(i + 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                // printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
                // If the seached cell is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i + 1][j] == false &&
                     isUnBlocked(grid, i + 1, j) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i + 1][j].f == FLT_MAX ||
                    cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(fNew,
                                              make_pair(i + 1, j)));

                    // Update the details of this cell
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        /////////EAST/////////

        if (isValid(i, j + 1) == true) {
            // If the destination cell is the same as the
            // searched cell
            if (isDestination(i, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                //
                // printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
                // If the seached cell is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i][j + 1] == false &&
                     isUnBlocked(grid, i, j + 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i][j + 1].f == FLT_MAX ||
                    cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew,
                                              make_pair(i, j + 1)));

                    // Update the details of this cell
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        /////////WEST/////////

        if (isValid(i, j - 1) == true) {
            // If the destination cell is the same as the
            // searched cell
            if (isDestination(i, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                // printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
                // If the seached cell is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i][j - 1] == false &&
                     isUnBlocked(grid, i, j - 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i][j - 1].f == FLT_MAX ||
                    cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew,
                                              make_pair(i, j + 1)));

                    // Update the details of this cell
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

        /////////NORTH-EAST/////////

        if (isValid(i - 1, j + 1) == true) {
            // If the destination cell is the same as the
            // searched cell
            if (isDestination(i - 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                //printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
                // If the seached cell is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i - 1][j + 1] == false &&
                     isUnBlocked(grid, i - 1, j + 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i - 1][j + 1].f == FLT_MAX ||
                    cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew,
                                              make_pair(i - 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }

        /////////NORTH-WEST/////////

        if (isValid(i - 1, j - 1) == true) {
            // If the destination cell is the same as the
            // searched cell
            if (isDestination(i - 1, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                //printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
                // If the seached cell is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i - 1][j - 1] == false &&
                     isUnBlocked(grid, i - 1, j - 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i - 1][j - 1].f == FLT_MAX ||
                    cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew,
                                              make_pair(i - 1, j - 1)));

                    // Update the details of this cell
                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }

        /////////SOUTH-EAST/////////

        if (isValid(i + 1, j + 1) == true) {
            // If the destination cell is the same as the
            // searched cell
            if (isDestination(i + 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                // printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
                // If the seached cell is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i + 1][j + 1] == false &&
                     isUnBlocked(grid, i + 1, j + 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i + 1][j + 1].f == FLT_MAX ||
                    cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew,
                                              make_pair(i + 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }

        /////////SOUTH-WEST/////////

        if (isValid(i + 1, j - 1) == true) {
            // If the destination cell is the same as the
            // searched cell
            if (isDestination(i + 1, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                // printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
                // If the seached cell is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i + 1][j - 1] == false &&
                     isUnBlocked(grid, i + 1, j - 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i + 1][j - 1].f == FLT_MAX ||
                    cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew,
                                              make_pair(i + 1, j - 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }

    if (foundDest == false) {
        // printf("can't find dest");
    }


    return;


}

std::string Chaser::getName() const {
    return "Chaser";
}
