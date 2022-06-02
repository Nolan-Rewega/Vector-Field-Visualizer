#include"Graph.h"


/* classic cartesean system, 4 quadrants */
Graph::Graph(int rows, int cols, int aisles, Math* mathObj){
    
    ROWS = rows;
    COLS = cols;
    AISLES = aisles;

    /* Internal bounds of the grid */
     left_x = -5.0;      right_x = 5.0;
     bot_y  = -5.0;      top_y   = 5.0;
     back_z = -5.0;      front_z = 5.0;

    /* discretization values */
    discretization_x = (right_x - left_x) / COLS;    
    discretization_y = (top_y - bot_y) / ROWS;
    discretization_z = (front_z - back_z) / AISLES;
    
    solver = mathObj;

    // --  Initilization of the Grid.
    initFieldData();
    updateGraph();
}
 
Graph::~Graph(){
    free(field_data);
}
 

void Graph::updateGraph() {
    for (int r = 0; r < ROWS - 1; r++) {
        for (int c = 0; c < COLS - 1; c++) {
            field_data[((COLS - 1) * r + c) * 3 + 0] = (left_x + (c * discretization_x));
            field_data[((COLS - 1) * r + c) * 3 + 1] = (top_y - (r * discretization_y));
            field_data[((COLS - 1) * r + c) * 3 + 2] = 0.0;
        }
    }
}

void Graph::calculateField(){
    double* results = nullptr;
    for (int i = 0; i < (ROWS - 1) * (COLS - 1) * 3; i += 3) {
        results = solver->parseToPostFix(field_data[i + 0], field_data[i + 1], field_data[i + 2]);
        field_data[i + 0] = results[0];
        field_data[i + 1] = results[1];
        field_data[i + 2] = results[2];
    }
}




void Graph::scaleGraph(double zoom_x, double zoom_y){
    // -- On invalid input exit.
    if (zoom_x == 0.0 || zoom_y == 0.0) { return; }

    // -- Scale X-axis.
    right_x *= zoom_x;
    left_x *= zoom_x;

    // -- Scale X-axis.
    top_y *= zoom_y;
    bot_y *= zoom_y;

    discretization_x = (right_x - left_x) / COLS;
    discretization_y = (top_y - bot_y) / ROWS;

    updateGraph();
    calculateField();
}


void Graph::translateGraph(double deltaX, double deltaY){
    // -- translate X-axis
    left_x += deltaX;
    right_x += deltaX;

    // -- Translate Y-axis
    top_y += deltaY;
    bot_y += deltaY;

    updateGraph();
    calculateField();
}




void Graph::printGraph(){
    for(int i = 0; i < field_data_size; i += 3){
        int idx = i / 3.0;
        printf("<%.0f, %3.0f, %3.0f>,      ", field_data[i+0], field_data[i+1], field_data[i+2]);
        if ((idx + 1) % (COLS - 1) == 0) { printf("\n"); }
    }
    printf("\n");
}

void Graph::initFieldData(){
    field_data_size = ((double)COLS - 1) * ((double)ROWS - 1) * 3;
    field_data_size_bytes = field_data_size * sizeof(double);
    field_data = (double*)calloc(field_data_size, sizeof(double));
}


