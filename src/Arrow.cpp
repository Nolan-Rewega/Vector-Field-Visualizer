#include "Arrow.h"

Arrow::Arrow(glm::vec3 normalizedVector, GLfloat w, GLfloat h){
	width = w;
	height = h;
	length = sqrt(normalizedVector.x * normalizedVector.x + normalizedVector.y * normalizedVector.y);


	bot = 0.0f - height / 2.0f;
	top = 0.0f + height / 2.0f;
	left = 0.0f - width / 6.0f;
	right = 0.0f + width / 6.0f;

	// -- 12 GLfloats per line, 3 lines per 1 Arrow.
	vertexDataSizeBytes = 12.0f * 3.0f * sizeof(GLfloat);
	vertexData = (GLfloat*)calloc(12.0f * 3.0f, sizeof(GLfloat));

	theta = atan(normalizedVector.x / normalizedVector.y);
	theta += (normalizedVector.y < 0.0f) ? 3.14f : 0.0f;

	fillVertexData();
	rotateArrow(glm::vec3(0.0f, 0.0f, 1.0f), -theta);
}

Arrow::~Arrow(){
	free(vertexData);
}



GLfloat* Arrow::getVertexData(){ return vertexData; }
int Arrow::getVertexDataSizeBytes(){ return vertexDataSizeBytes; }
glm::mat4 Arrow::getTranslationMatrix() { return translationMatrix; }
glm::mat4 Arrow::getRotationMatrix() { return rotationMatrix; }


void Arrow::rotateArrow(glm::vec3 angleVec, GLfloat angle){
	rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, angleVec);
	fillVertexData();
}

void Arrow::translateArrow(glm::vec3 targetPos) {
	translationMatrix = glm::translate(glm::mat4(1.0f), targetPos);
	fillVertexData();
}



void Arrow::fillVertexData(){
	// -- arrow defined at origin.
	for (int i = 3; i < 12.0 * 3.0; i += 6) {
		vertexData[i+0] = 0.0f; 
		vertexData[i+1] = 0.0f;	 
		vertexData[i+2] = length;
	}

	vertexData[0] = ArrowCenterLocation[0]; 	vertexData[1] = bot;	 vertexData[2] = 0.0f;
	vertexData[6] = ArrowCenterLocation[0]; 	vertexData[7] = top;	 vertexData[8] = 0.0f;

	vertexData[12] = left; 						vertexData[13] = top/2;	 vertexData[14] = 0.0f;
	vertexData[18] = ArrowCenterLocation[0]; 	vertexData[19] = top;	 vertexData[20] = 0.0f;

	vertexData[24] = right; 					vertexData[25] = top/2;	 vertexData[26] = 0.0f;
	vertexData[30] = ArrowCenterLocation[0]; 	vertexData[31] = top;	 vertexData[32] = 0.0f;

}

