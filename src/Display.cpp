#include"Display.h"



Display::Display(Camera* cam){  
	
	// -- Set Camera reference.
	camera = cam;

	// -- Initialization.
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "VFV", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 600);

	// -- Read in shaders.
	std::string VSSinput = readShaderCode("vertexshader.glsl");
	std::string VSS2input = readShaderCode("border.glsl");
	std::string FSSinput = readShaderCode("fragmentshader.glsl");
	const char* VSS = VSSinput.c_str();
	const char* VSS2 = VSS2input.c_str();
	const char* FSS = FSSinput.c_str();


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	program2 = glCreateProgram();

	// -- Compilation and linking.
	glShaderSource(vertexShader, 1, &VSS, NULL);
	glCompileShader(vertexShader);

	glShaderSource(vertexShader2, 1, &VSS2, NULL);
	glCompileShader(vertexShader2);

	glShaderSource(fragmentShader, 1, &FSS, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glAttachShader(program2, vertexShader2);
	glAttachShader(program2, fragmentShader);
	glLinkProgram(program2);

	// -- Delete shaders after linking.
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShader2);
	glDeleteShader(fragmentShader);

}

void Display::drawGraph(Graph* graph) {
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// -- Draw the Graphs borders.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, graph->getGraphBordersSizeBytes(), graph->getGraphBorders(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	glUseProgram(program2);
	glBindVertexArray(VAO);
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, 8);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	for (Arrow* arrow : graph->getArrows()) {
		drawArrow(arrow);
	}

	// -- Swap front and back buffers
	glfwSwapBuffers(window);
}


void Display::drawArrow(Arrow* arrow) {
	// -- Draw the Graphs vectors
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, arrow->getVertexDataSizeBytes(), arrow->getVertexData(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrow->getIndiceDataSizeBytes(), arrow->getIndiceData(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);


	// -- matrix operations
	glm::mat4 projectionMat4 = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 10.0f);
	glm::mat4 transform = projectionMat4 * camera->getWorldToViewMatrix() * arrow->getTranslationMatrix() * arrow->getRotationMatrix();

	GLint transformMat4Loc = glGetUniformLocation(program, "transformMat4");
	glUniformMatrix4fv(transformMat4Loc, 1, GL_FALSE, &transform[0][0]);


	glUseProgram(program);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_SHORT, 0);


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

GLFWwindow* Display::getWindow() {
	return window;
}

void Display::pollEvents(){
    glfwPollEvents();
}


bool Display::checkTermination(){
    return glfwWindowShouldClose(window);
}


void Display::exit(){
	/* Delete buffers and clean up */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();
}

string Display::readShaderCode(const char* filename) {
	string shader, line;
	ifstream inputFile(filename);
	if (!inputFile.good()) {
		cout << "FAILED TO LOAD FILE: " << filename << " IN readSHaderCode()" << endl;
		exit();
	}
	while (getline(inputFile, line)) {
		shader.append(line);
		shader.append("\n");
	}
	return shader;
}
