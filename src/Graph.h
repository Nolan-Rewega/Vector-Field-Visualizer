#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

#include"Math.h"
#include"Arrow.h"
#include"Shape.h"

using namespace std;

class Graph : public Shape{
public:
    /* Methods */
    Graph(int rows, int cols, int aisles, Math* mathobj);
    ~Graph();

    vector<Arrow*> getArrows();

    void updateGraph();
    void calculateField();



private:
    // -- Constant definitions.
    int ROWS;
    int COLS;
    int AISLES;

    // -- initial definitions of x-axis, y-axis, z-axis, bounds.
    GLfloat left_x, bot_y, right_x, top_y, front_z, back_z;
    GLfloat discretization_x, discretization_y, discretization_z;

    std::vector<Arrow*> arrow_data;

    // -- store vector data
    GLfloat* field_data;
    int field_data_size;
    GLfloat field_data_size_bytes;

    // -- solver object
    Math* solver;

    void fillBorderData();
    void initFieldData();
};
#endif //!GRAPH_H

