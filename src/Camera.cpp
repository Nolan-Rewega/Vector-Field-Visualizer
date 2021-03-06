#include "Camera.h"

Camera::Camera(GLfloat sensitivity, GLfloat sphereRadius){
	radius = sphereRadius;
	theta = glm::radians(180.0f);
	phi = glm::radians(-90.0f);

	// -- origin of the camera sphere
	origin = glm::vec3(0.0, -3.0, 0.0f);


	eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	viewDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	SENSE = sensitivity;

	updateParameters();
}

glm::mat4 Camera::getWorldToViewMatrix(){
	return glm::lookAt(eyePosition, eyePosition + viewDirection, upVector);
}

void Camera::sphereRotation(GLfloat dTheta, GLfloat dPhi) {
	theta += dTheta * SENSE;
	if (phi - dPhi < -0.00001 && phi - dPhi > -3.14159) {
		phi -= dPhi * SENSE;
	}
	updateParameters();
}
 
void Camera::zoom(GLfloat dZoom){
	radius -= dZoom;
	updateParameters();
}


void Camera::updateParameters(){
	eyePosition = glm::vec3(
		radius * glm::sin(phi) * glm::sin(theta),
		radius * glm::cos(phi),
		radius * glm::sin(phi) * glm::cos(theta)
	) + origin;

	// -- view direction is the normal vector of sphere surface
	viewDirection = -eyePosition + origin;
}



