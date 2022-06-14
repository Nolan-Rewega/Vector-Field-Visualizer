#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <glm/glm.hpp>


class TextLabel{
public:

	TextLabel(glm::vec3 givenOrigin, glm::vec3 givenColor, float givenWidth, std::string givenText);

	glm::vec3 origin, color;
	float width;
	std::string text;

};
#endif //!TEXT_H
