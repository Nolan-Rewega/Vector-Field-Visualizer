#ifndef DISPLAY_H
#define DISPLAY_H

#include<iostream>
#include<fstream>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ft2build.h>

#include"Graph.h"
#include"Arrow.h"
#include"Camera.h"
#include"TextLabel.h"

#include FT_FREETYPE_H 

using namespace std;

class Display{
public:

    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };
    std::map<char, Character> Characters;


    /* Attributes*/
	GLFWwindow* window;
	GLuint program, textProgram;
    GLuint VAO, VBO, EBO;
    GLuint TVAO, TVBO;


    Camera* camera;
    glm::mat4 projectionMat4;

    /* Methods */
    Display(Camera* cam);
    bool checkTermination();
    void drawGraph(Graph* graph);
    void drawArrow(Arrow* arrow);
    void drawText(TextLabel* text);
    GLFWwindow* getWindow();
    void pollEvents();
    void exit();

private:
    string readShaderCode(const char* filename);
};
#endif //!DISPLAY_H

