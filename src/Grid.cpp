#include"Grid.h"


/* classic cartesean grid, 4 quadrants */
Grid::Grid(){
    
    /* inital bounds of the grid*/
    left_x = -5.0; bot_y = -5.0;
    right_x = 5.0; top_y = 5.0;
    
    /* discretization values */
    discretization_x = (right_x - left_x) / GRIDCOLS;    
    discretization_y = (top_y - bot_y) / GRIDROWS;
    
    results.resize(GRIDROWS*GRIDCOLS, vector<double>(4, 0.0))   
    
    // -- get Inital equations
    solver.getInput();

    // --  Initilization of the Grid.
    updateGrid();
}
 
 
void Grid::updateGrid(){
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


void Grid::gridShift(double shift_x, double shift_y){
    // shift the x-axis
    right_x = right_x + shift_x;
    left_x = left_x + shift_x;
    // shift y-axis
    top_y = top_y + shift_y;
    bot_y = bot_y + shift_y;

    updateGrid();
}
 

void Grid::gridScale(double zoom_x, double zoom_y){
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
    updateGrid();
}
    

void Grid::verticalTransform(double top_bound, double bot_bound){
    top_y = top_bound; bot_y = bot_bound;
    discretization_y = (top_y - bot_y) / GRIDROWS;
    
    updateGrid();
}


void Grid::horizontalTransform(double left_bound, double right_bound){
    left_x = left_bound; right_x = right_bound;
    discretization_x = (right_x - left_x) / GRIDCOLS;
    
    updateGrid();
}


void Grid::printGrid(){
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


