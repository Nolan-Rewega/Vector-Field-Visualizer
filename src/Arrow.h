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

	GLfloat* vertexData;
	GLfloat  vertexDataSizeBytes;

	glm::vec3 ArrowCenterLocation;

	GLfloat width, height, length;
	GLfloat top, bot, left, right, front, back;
	GLfloat theta;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;

	Arrow(glm::vec3 normalizedVector, GLfloat width, GLfloat height);
	~Arrow();

	GLfloat* getVertexData();
	int getVertexDataSizeBytes();
	glm::mat4 getTranslationMatrix();
	glm::mat4 getRotationMatrix();

	void rotateArrow(glm::vec3 angleVec, float angle);
	void translateArrow(glm::vec3 targetPos);

private:
	void fillVertexData();
};



#endif // !ARROW_H


