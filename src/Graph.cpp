#include"Graph.h"


/* classic cartesean grid, 4 quadrants */
Graph::Graph(){
    
    /* Internal bounds of the grid */
    left_x = -5.0; bot_y = -5.0;
    right_x = 5.0; top_y = 5.0;
    
    /* discretization values */
    discretization_x = (right_x - left_x) / GRIDCOLS;    
    discretization_y = (top_y - bot_y) / GRIDROWS;
    
    vector_data.resize(GRIDROWS * GRIDCOLS, vector<double>(4, 0.0));
    
    // -- get Inital equations
    solver.getInput();

    // --  Initilization of the Grid.
    updateGraph();
}
 
 
void Graph::updateGraph(){
    double x_value, y_value;
    
    // -- erase the previous data for new data
    vector_data.erase();
    
    /* passes each value (x,y) value to parse to postfix to solve the eq */
    for(int row = 0; row < GRIDROWS; row++){
        vector<double> data;
        for(int col = 0; col < GRIDCOLS; col++){
            /* Generate x and y values based on bounds and discretization */
            x_value = (left_x + (row * discretization_x));
            y_value = (top_y - (col * discretization_y));
            solver.parseToPostFix(x_value, y_value, 0);
            
            data.push_back(x_value);
            data.push_back(y_value);
            data.push_back(x_value + solver.results[0]);
            data.push_back(y_value + solver.results[1]);
        }
        vector_data.push_back(data);
    }
}



void Graph::scaleGraph(double zoom_x, double zoom_y){
    // scale x-axis
    if(zoom_x != 0){
        right_x = right_x * zoom_x;
        left_x = left_x * zoom_x;
        discretization_x = (right_x - left_x) / GRIDCOLS;
    }
    // scale y-axis
    if(zoom_y != 0){
        top_y = top_y * zoom_y;
        bot_y = bot_y * zoom_y;
        discretization_y = (top_y - bot_y) / GRIDROWS;
    }
    updateGraph();
}

void Graph::translateGraph(double deltaX, double deltaY){
    // -- translate X-axis
    left_x += deltaX;
    right_x += deltaX;

    // -- Translate Y-axis
    top_y += deltaY;
    bot_y += deltaY;

    updateGraph();
}


void Graph::printGraph(){
    for(int row = 0; row < GRIDROWS; row++){
        cout << "[ ";
        for(int col = 0; col < GRIDCOLS; col++){
            cout << "(";
            cout << (left_x + (row * discretization_x)) << ",";
            cout << (top_y - (col * discretization_y));
            cout << ") ";
        }
        cout << " ]" << endl;
    }
}


