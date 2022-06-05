#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>

#include"Math.h"
#include"Arrow.h"

using namespace std;

class Graph{
public:
    /* Methods */
    Graph(int rows, int cols, int aisles, Math* mathobj);
    ~Graph();

    int getGraphBordersSizeBytes();
    GLfloat* getGraphBorders();
    vector<Arrow*> getArrows();



    void updateGraph();
    void calculateField();
    void scaleGraph(GLfloat zoom_x, GLfloat zoom_y);
    void translateGraph(GLfloat deltaX, GLfloat deltaY);
    void printGraph();

private:
    // -- Constant definitions.
    int ROWS;
    int COLS;
    int AISLES;

    // -- initial definitions of x-axis, y-axis, z-axis, bounds.
    GLfloat left_x, bot_y, right_x, top_y, front_z, back_z;
    GLfloat discretization_x, discretization_y, discretization_z;

    // -- store 2D vector data
    GLfloat* field_data;
    GLfloat field_data_size;
    GLfloat field_data_size_bytes;

    vector<Arrow*> arrow_data;

    GLfloat* border_data;

    // -- solver object
    Math* solver;


    void initFieldData();
    void fillBorderData();
};
#endif //!GRID_H

