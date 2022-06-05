#ifndef DISPLAY_H
#define DISPLAY_H

#include<iostream>
#include<fstream>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Graph.h"
#include"Arrow.h"

using namespace std;

class Display{
public:
    /* Attributes*/
	GLFWwindow* window;
	GLuint program, program2;
    GLuint VAO, VBO;
    
    /* Methods */
    Display();
    bool checkTermination();
    void drawGraph(Graph* graph);
    void drawArrow(Arrow* arrow);
    void pollEvents();
    void exit();

private:
    string readShaderCode(const char* filename);
};
#endif //!DISPLAY_H

