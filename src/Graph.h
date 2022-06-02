#ifndef GRID_H
#define GRID_H

#include <iostream>
#include"Math.h"
using namespace std;

class Graph{
public:
    // -- Constant definitions.
    int ROWS;
    int COLS;
    int AISLES;
    
    // -- initial definitions of x-axis, y-axis, z-axis, bounds.
    double left_x, bot_y, right_x, top_y, front_z, back_z;
    double discretization_x, discretization_y, discretization_z;
    
    // -- store 2D vector data
    //vector<vector<double>> vector_data;
    double* field_data;
    double field_data_size;
    double field_data_size_bytes;

    // -- solver object
    Math solver;


    /* Methods */
    Graph(int rows, int cols, int aisles);
    ~Graph();
    void updateGraph();
    void scaleGraph(double zoom_x, double zoom_y);
    void translateGraph(double deltaX, double deltaY);
    void printGraph();

private:
    void initFieldData();

};
#endif //!GRID_H

