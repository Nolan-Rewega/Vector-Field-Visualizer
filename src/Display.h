#ifndef DISPLAY_H
#define DISPLAY_H

#define GLFW_INCLUDE_NONE
#include<iostream>
#include<fstream>
#include<string>

#include<GLFW/glfw3.h>
#include<glad/glad.h>
//#include"linmath.h"

#include"Graph.h"

using namespace std;

class Display{
public:
    /* Attributes*/
	GLFWwindow* window;
	GLuint vertexShader, fragmentShader, program;
    GLuint VAO, VBO;
    
    /* Methods */
    Display();
    bool check_termination();
    void draw_graph(Graph* graph);
    void poll_events();
    void exit();

private:
    string readShaderCode(const char* filename);
};
#endif //!DISPLAY_H

