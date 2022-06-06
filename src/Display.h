#ifndef DISPLAY_H
#define DISPLAY_H

#include<iostream>
#include<fstream>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Graph.h"
#include"Arrow.h"
#include"Camera.h"

using namespace std;

class Display{
public:
    /* Attributes*/
	GLFWwindow* window;
	GLuint program, program2;
    GLuint VAO, VBO, EBO;
    
    Camera* camera;

    /* Methods */
    Display(Camera* cam);
    bool checkTermination();
    void drawGraph(Graph* graph);
    void drawArrow(Arrow* arrow);
    GLFWwindow* getWindow();
    void pollEvents();
    void exit();

private:
    string readShaderCode(const char* filename);
};
#endif //!DISPLAY_H

