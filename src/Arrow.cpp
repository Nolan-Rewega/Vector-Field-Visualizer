#include "Arrow.h"

Arrow::Arrow(glm::vec3 vector, GLfloat w, GLfloat h, GLfloat l){
	width = w;
	height = h;
	length = l;
	magnitude = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

	// -- calculate color based on magnitude
	if (magnitude < 5) {
		blue = magnitude / 5.0f;
	}
	else if (magnitude < 20) {
		blue = magnitude / 5.0f;
		red = magnitude / 20.0f;
	}
	else {
		blue = magnitude / 5.0f;
		red = magnitude / 20.0f;
		green = magnitude / 50.0f;
	}

	// -- calculate rotation angles.
	theta = atan(vector.x / vector.y);

	phi = atan(vector.z / vector.y);
	phi += (vector.y < 0.0f) ? 3.14f : 0.0f;

	// -- 13 vertices per Arrow, 6 components per vertex.
	numberOfVertices = 13;
	vertexDataByteSize = numberOfVertices * 6 * sizeof(GLfloat);
	vertexData = (GLfloat*)calloc(numberOfVertices * 6, sizeof(GLfloat));

	// -- 12 Lines per border, 2 Indices per line.
	numberOfDataIndices = 48;
	dataIndicesByteSize = numberOfDataIndices * sizeof(GLushort);
	dataIndices = (GLushort*)calloc(dataIndicesByteSize, sizeof(GLushort));
	GLushort test[48] =
	{
		0, 2, 3,	1, 0, 3,	4,  0, 1,	5,  4, 1,
		6, 2, 4,	4, 3, 0,	6,  7, 3,	6,  3, 2,
		5, 1, 3,	5, 3, 7,	8, 10,11,	8, 11, 9,
		12,8, 9,	12,10,8,	12,11,10,	12, 9,11
	};
	for (int i = 0; i < 48; i++) { dataIndices[i] = test[i];}

	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
	rotationMatrix = glm::mat4(1.0f);


	fillVertexData();
	rotateShape(glm::vec3(0.0f, 0.0f, 1.0f), -theta);
	rotateShape(glm::vec3(1.0f, 0.0f, 0.0f), phi);

}

Arrow::~Arrow(){
	freeVertexData();
}




void Arrow::fillVertexData(){
	// -- Arrow sizes
	GLfloat quarterW = width / 4.0f;
	GLfloat eighthW = width / 8.0f;

	GLfloat halfH = height / 2.0f;
	GLfloat eighthH = height / 8.0f;

	GLfloat eighthL = length / 8.0f;

	// -- fill in color.
	for (int i = 3; i < 13.0 * 6.0; i += 6) {
		vertexData[i+0] = red; 
		vertexData[i+1] = green;	 
		vertexData[i+2] = blue;
	}

	// -- arrow defined at origin.
	// -- Hard coded Vertices
	//              X                            Y                           Z
	vertexData[0]  =  -eighthW; 	vertexData[1]  =  -halfH;	 vertexData[2]  =  eighthL; // -- base bottom
	vertexData[6]  =   eighthW; 	vertexData[7]  =  -halfH;	 vertexData[8]  =  eighthL;
	vertexData[12] =  -eighthW; 	vertexData[13] =  -halfH;	 vertexData[14] = -eighthL;
	vertexData[18] =   eighthW; 	vertexData[19] =  -halfH;	 vertexData[20] = -eighthL;

	vertexData[24] =  -eighthW; 	vertexData[25] = eighthH;	 vertexData[26] =  eighthL; // -- base top
	vertexData[30] =   eighthW; 	vertexData[31] = eighthH;	 vertexData[32] =  eighthL;
	vertexData[36] =  -eighthW; 	vertexData[37] = eighthH;	 vertexData[38] = -eighthL;
	vertexData[42] =   eighthW; 	vertexData[43] = eighthH;	 vertexData[44] = -eighthL; 

	vertexData[48] = -quarterW; 	vertexData[49] = eighthH;	 vertexData[50] =  eighthL; // -- head bottom
	vertexData[54] =  quarterW; 	vertexData[55] = eighthH;	 vertexData[56] =  eighthL;
	vertexData[60] = -quarterW; 	vertexData[61] = eighthH;	 vertexData[62] = -eighthL;
	vertexData[66] =  quarterW; 	vertexData[67] = eighthH;	 vertexData[68] = -eighthL;
	
	vertexData[72] =      0.0f; 	vertexData[73] =   halfH;	 vertexData[74] = 0.0f; // -- head top
}

