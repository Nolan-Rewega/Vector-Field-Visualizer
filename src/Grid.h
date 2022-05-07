#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include"Math.h"
using namespace std;

class Grid{
public:
    // -- constant definitions.
    const int GRIDROWS = 10;
    const int GRIDCOLS = 10;
    
    // -- initial definitions of x-axis, y-axis bounds.
    double left_x, bot_y, right_x, top_y;
    double discretization_x, discretization_y;
    
    // -- create a 2D results vector data
    vector<vector<double>> vector_data;

    // -- solver object
    Math solver;


    /* Methods*/
    Grid();
    void updateGrid();
    void gridScale(double zoom_x, double zoom_y);
    void gridShift(double shift_x, double shift_y);
    void horizontalTransform(double left_bound, double right_bound);
    void verticalTransform(double top_bound, double bot_bound);
    void printGrid();

};
#endif

