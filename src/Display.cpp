#define GLFW_INCLUDE_NONE
#include"Display.h"

Display::Display(){    
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
	std::string FSSinput = readShaderCode("fragmentshader.glsl");
	const char* VSS = VSSinput.c_str();
	const char* FSS = FSSinput.c_str();


	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

	// -- Compilation and linking.
	glShaderSource(vertexShader, 1, &VSS, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &FSS, NULL);
	glCompileShader(fragmentShader);
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// -- Delete shaders after linking.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}



void Display::draw_graph(Graph* graph){
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -- Draw the Graphs borders.



    glUseProgram(program);
    glBindVertexArray(VAO);
    glLineWidth(4);
    glDrawArrays(GL_LINES, 0, 4);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// -- Draw the Graphs vectors




	glUseProgram(program);
	glBindVertexArray(VAO);
	glLineWidth(4);
	glDrawArrays(GL_LINES, 0, 4);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// -- Swap front and back buffers
	glfwSwapBuffers(window);
}


void Display::poll_events(){
    glfwPollEvents();
}


bool Display::check_termination(){
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
