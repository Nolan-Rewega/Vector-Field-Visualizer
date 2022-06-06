#include"Graph.h"
#include"Math.h"
#include"Arrow.h"

/* classic cartesean system, 4 quadrants */
Graph::Graph(int rows, int cols, int aisles, Math* mathObj){
    
    ROWS = rows;
    COLS = cols;
    AISLES = aisles;

    /* Internal bounds of the grid */
     left_x = -5.0f;      right_x = 5.0f;
     bot_y  = -5.0f;      top_y   = 5.0f;
     back_z = -5.0f;      front_z = 5.0f;

    /* discretization values */
    discretization_x = (right_x - left_x) / COLS;    
    discretization_y = (top_y - bot_y) / ROWS;
    discretization_z = (front_z - back_z) / AISLES;
    
    solver = mathObj;

    border_data = (GLfloat*)calloc(12.0f * 4.0f, sizeof(GLfloat));
    fillBorderData();


    // --  Initilization of the Grid.
    initFieldData();
    updateGraph();
}
 
Graph::~Graph(){
    free(field_data);
    free(border_data);
}
 

int Graph::getGraphBordersSizeBytes() {
    return 12.0f * 4.0f * sizeof(GLfloat);
}
GLfloat* Graph::getGraphBorders() {
    return border_data;
}

vector<Arrow*> Graph::getArrows() {
    return arrow_data;
}


void Graph::updateGraph() {
    for (int r = 0; r < ROWS - 1; r++) {
        for (int c = 0; c < COLS - 1; c++) {
            field_data[((COLS - 1) * r + c) * 3 + 0] = (left_x + ((c+1) * discretization_x));
            field_data[((COLS - 1) * r + c) * 3 + 1] = (top_y - ((r+1) * discretization_y));
            field_data[((COLS - 1) * r + c) * 3 + 2] = 0.0f;
        }
    }
}


void Graph::calculateField(){
    arrow_data.clear();
    double* results = nullptr;

    for (int i = 0; i < (ROWS - 1) * (COLS - 1) * 3; i += 3) {
        GLfloat col = (field_data[i + 0] - left_x) / discretization_x;
        GLfloat row = (field_data[i + 1] - top_y) / (- discretization_y);

        glm::vec3 wrldpos = glm::vec3(-0.8f + (col * 1.6f / COLS), 0.8f - (row * 1.6f / ROWS), 0.0f);

        results = solver->parseToPostFix(field_data[i + 0], field_data[i + 1], field_data[i + 2]);
        Arrow* a = new Arrow(glm::vec3(results[0], results[1], 0.0f), 1.6f/COLS, 1.6f/ROWS, 0.1f);
        a->translateArrow(wrldpos);
        arrow_data.push_back(a);

    }
}




void Graph::scaleGraph(GLfloat zoom_x, GLfloat zoom_y){
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


void Graph::translateGraph(GLfloat deltaX, GLfloat deltaY){
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
    field_data_size = ((GLfloat)COLS - 1) * ((GLfloat)ROWS - 1) * 3;
    field_data_size_bytes = field_data_size * sizeof(GLfloat);
    field_data = (GLfloat*)calloc(field_data_size, sizeof(GLfloat));
}

void Graph::fillBorderData(){
    for (int i = 3; i < 12.0f * 4.0f; i += 6) {
        border_data[i + 0] = 1.0f;
        border_data[i + 1] = 1.0f;
        border_data[i + 2] = 1.0f;
    }

    border_data[0] = -0.8f; 	border_data[1] = 0.8f;	 border_data[2] = 0.0f;
    border_data[6] = -0.8f; 	border_data[7] = -0.8f;	 border_data[8] = 0.0f;

    border_data[12] = 0.8f; 	border_data[13] = 0.8f; border_data[14] = 0.0f;
    border_data[18] = 0.8f; 	border_data[19] = -0.8f; border_data[20] = 0.0f;

    border_data[24] = -0.8f; 	border_data[25] = 0.8f;	 border_data[26] = 0.0f;
    border_data[30] = 0.8f; 	border_data[31] = 0.8f;  border_data[32] = 0.0f;

    border_data[36] = -0.8f; 	border_data[37] = -0.8f; border_data[38] = 0.0f;
    border_data[42] = 0.8f; 	border_data[43] = -0.8f; border_data[44] = 0.0f;
}



