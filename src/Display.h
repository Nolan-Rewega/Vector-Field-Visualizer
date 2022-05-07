#ifndef DISPLAY_H
#define DISPLAY_H

#define GLFW_INCLUDE_NONE
#include<iostream>
#include<GLFW/glfw3.h>
#include<glad/gl.h>
#include"linmath.h"

using namespace std;

class Display{
public:
    // Vertex Shader source code
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    //Fragment Shader source code
    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";


    /* Attributes*/
	GLFWwindow* window;
	GLuint vertexShader, fragmentShader, program;
    GLuint VAO, VBO;
	GLfloat* vertices;
    bool status;
    
    /* Methods */
    Display();
    bool check_termination();
    bool init();
    void update_vertices();
    void plot_vectors();
    void background();
    void poll_events();
    void exit();  
};
#endif

