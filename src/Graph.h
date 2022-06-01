#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include"Math.h"
using namespace std;

class Graph{
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


    /* Methods */
    Graph();
    void updateGraph();
    void scaleGraph(double zoom_x, double zoom_y);
    void translateGraph(double deltaX, double deltaY);
    void printGraph();

};
#endif

