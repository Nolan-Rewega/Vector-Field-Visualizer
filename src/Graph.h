#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <vector>


#include"Math.h"
#include"Arrow.h"
#include"Shape.h"
#include"TextLabel.h"

using namespace std;

class Graph : public Shape{
public:
    /* Methods */
    Graph(int rows, int cols, int aisles, Math* mathobj, bool is2D);
    ~Graph();

    vector<Arrow*> getArrows();
    vector<TextLabel*> getText();
    
    void translateReferenceFrame(glm::vec3 delta);
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
    std::vector<TextLabel*> text;

    // -- store vector data
    GLfloat* field_data;
    int field_data_size;
    GLfloat field_data_size_bytes;

    // -- solver object
    Math* solver;

    void fillBorderData();
    void fillTextLabels();
    void initFieldData();
};
#endif //!GRAPH_H

