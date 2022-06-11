#include<iostream>
#include"Math.h"
#include"Graph.h"
#include"Display.h"
#include"Camera.h"

using namespace std;

static void handleMouseMovement(GLFWwindow* window, double xpos, double ypos);
static void handleMouseClick(GLFWwindow* window, int button, int action, int mods);
static void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateGraph();
void getUserInput();

// -- Yucky globals :(

Camera* camera = new Camera(0.005f, 1.5f);
GLfloat prevX = 0.0f;
GLfloat prevY = 0.0f;

Math* mathObj = new Math();

Graph* field;
int ARROWS = 7;
int dim;

Display* display;

bool DRAGGING = false;



int main(){
    // -- Getting intial input.
    getUserInput();
    updateGraph();

    display = new Display(camera);
    display->drawGraph(field);

    // -- set callback functions.
    glfwSetCursorPosCallback(display->getWindow(), handleMouseMovement);
    glfwSetMouseButtonCallback(display->getWindow(), handleMouseClick);
    glfwSetKeyCallback(display->getWindow(), handleKeyPress);

    // --  Main work loop.
    while(!display->checkTermination()){
        // -- Event based Rendering. 
        display->pollEvents();
    }
    display->exit();



    ///* Testing Math parser */
    //Math test;
    //string a1 = test.shunting("3 + 4 * 2 ");
    //string a2 = test.shunting("3 + 4 + 2 * 5 ");
    //string a3 = test.shunting("3.14 + 4 * 2 ");
    //string a4 = test.shunting("3 + 4 * 2 ^ 2 ");
    //string a5 = test.shunting("25 - ( 3 * ( 2 + 3 ) / 5 ) ");
    //string a6 = test.shunting("( ( 4 + 2 ) * ( 2 / 2 ) ) ");
    //string a7 = test.shunting("6 / 3 + ( 5 ^ 2 ) ");  
    //string a8 = test.shunting("3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3 ");
    //string a9 = test.shunting("sin ( 3 * cos ( 4 + 4 ) ) ");
    //string a10 = test.shunting("( 4 + 5 ) ! ");
    //string a11 = test.shunting("( ln ( 16 ) + 5 ! ) / ( 3 * 6 ) ");
    //string a12 = test.shunting("sin ( 3 / 3 * 3.14 ) ");
    //
    //// testing evalInfix 
    //// 95% condident evalInfix works as intended 
    //cout << "RESULT OF (" << a1 << ") = " << test.evalPostfix(a1) << endl;
    //cout << "RESULT OF (" << a2 << ") = " << test.evalPostfix(a2) <<endl;
    //cout << "RESULT OF (" << a3 << ") = " << test.evalPostfix(a3) <<endl;
    //cout << "RESULT OF (" << a1 << ") = " << test.evalPostfix(a1) <<endl;
    //cout << "RESULT OF (" << a4 << ") = " << test.evalPostfix(a4) <<endl;
    //cout << "RESULT OF (" << a5 << ") = " << test.evalPostfix(a5) <<endl;
    //cout << "RESULT OF (" << a6 << ") = " << test.evalPostfix(a6) <<endl;
    //cout << "RESULT OF (" << a7 << ") = " << test.evalPostfix(a7) <<endl;
    //cout << "RESULT OF (" << a8 << ") = " << test.evalPostfix(a8) <<endl;
    //cout << "RESULT OF (" << a9 << ") = " << test.evalPostfix(a9) <<endl;
    //cout << "RESULT OF (" << a10<< ") = " << test.evalPostfix(a10) <<endl;
    //cout << "RESULT OF (" << a11<< ") = " << test.evalPostfix(a11) <<endl;
    //cout << "RESULT OF (" << a12<< ") = " << test.evalPostfix(a12) <<endl;

    delete field;
    delete camera;
    delete display;
    delete mathObj;

    return 1;
};





void getUserInput() {
    dim = mathObj->getDimensionality();
    mathObj->getInput();
}


void updateGraph() {
    delete field;
    field = new Graph(ARROWS, ARROWS, ARROWS, mathObj, (dim == 2));
    field->calculateField();
}





// -- Yucky functions in main :( .
static void handleMouseMovement(GLFWwindow* window, double xpos, double ypos){
    if (DRAGGING) {
        camera->sphereRotation(prevX - xpos, prevY - ypos);
        display->drawGraph(field);
    }
    prevX = xpos;
    prevY = ypos;
}

static void handleMouseClick(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        DRAGGING = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        DRAGGING = false;
    }
}

static void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) { 
        if (ARROWS < 15) { ARROWS++; }
        updateGraph();
        display->drawGraph(field);
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        if (ARROWS > 2) { ARROWS--; }
        updateGraph();
        display->drawGraph(field);
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        getUserInput();
        updateGraph();
        display->drawGraph(field);
    }

}


