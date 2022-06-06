#ifndef ARROW_H
#define ARROW_H
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

class Arrow{
public:

	Arrow(glm::vec3 normalizedVector, GLfloat width, GLfloat height, GLfloat length);
	~Arrow();

	GLfloat* getVertexData();
	int getVertexDataSizeBytes();
	GLushort* getIndiceData();
	int getIndiceDataSizeBytes();
	glm::mat4 getTranslationMatrix();
	glm::mat4 getRotationMatrix();

	void rotateArrow(glm::vec3 angleVec, float angle);
	void translateArrow(glm::vec3 targetPos);

private:

	GLfloat* vertexData;
	int  vertexDataSizeBytes;
	GLushort* indiceData;
	int  indiceDataSizeBytes;


	GLfloat red, green, blue, magnitude;
	GLfloat width, height, length;
	GLfloat theta, phi;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;


	void fillVertexData();
};



#endif // !ARROW_H


