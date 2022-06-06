#include<iostream>
#include"Math.h"
#include"Graph.h"
#include"Display.h"
#include"Camera.h"

static void handleMouseMovement(GLFWwindow* window, double xpos, double ypos);

using namespace std;

// -- Yucky globals
Camera* camera = new Camera(0.005f, 1.0f);
GLfloat prevX = 0.0f;
GLfloat prevY = 0.0f;

int main(){
    /* Intializing objects */
    Math* mathObj = new Math();
    mathObj->getInput();


    Graph* field = new Graph(25, 25, 0, mathObj);
    Display* display = new Display(camera);

    // -- set callback functions
    glfwSetCursorPosCallback(display->getWindow(), handleMouseMovement);

    // -- calculate field.
    field->calculateField();

    while(!display->checkTermination()){
        /* OpenGL Rendering */
        display->drawGraph(field);
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

    return 1;
};


// -- Yucky fuction location.
static void handleMouseMovement(GLFWwindow* window, double xpos, double ypos){
    cout << prevX - xpos << "   " << prevY - ypos << endl;
    
    camera->sphereRotation(prevX - xpos, prevY - ypos);
    prevX = xpos;
    prevY = ypos;
}



