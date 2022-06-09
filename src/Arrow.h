#ifndef ARROW_H
#define ARROW_H
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shape.h"

class Arrow : public Shape{
public:

	Arrow(glm::vec3 normalizedVector, GLfloat width, GLfloat height, GLfloat length);
	~Arrow();

private:
	GLfloat red, green, blue, magnitude;
	GLfloat width, height, length;
	GLfloat theta, phi;

	void fillVertexData();
};



#endif // !ARROW_H


