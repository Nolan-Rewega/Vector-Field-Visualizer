#define GLFW_INCLUDE_NONE
#include"Display.h"

Display::Display(){    
    bool status = init();
}

bool Display::init(){
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "VFV", NULL, NULL);
	if (window == NULL){
		cout << "Error in glfwCreateWindow.." << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glViewport(0, 0, 800, 600);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Vertices coordinates
	GLfloat points[] = {
	    1.0f,   0.0f,   0.0f, // Lower left corner
	    -1.0f,   0.0f,   0.0f, // Lower right corner

	    0.0f,   1.0f,   0.0f, // Lower left corner
		0.0f,   -1.0f,   0.0f, // Lower right corner
	};
    
    vertices = points;
    
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    return true;
}

void Display::update_vertices(vector<vector<double>> point_data){
    
	/* create a the vertex's for the vectors
        {   "start x", "start y", "end x", "end y"
            ...         ...         ...     ...
        }
    */
	GLfloat vectors[point_data.size() * 3];
    int counter = 0;

    /* get vector data from each point */
    for(int idx = 0; idx < point_data.size(); idx++){
        for(int item = 0; item < 3; item++){
            vectors[counter] = point_data[idx][item];
        }
    }
    // angle = sin(rise / sqrt(run^2 + rise^2))
    
    
    vertices = vectors;
    
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Display::background(){
    // -- pre
    glClearColor(0.08f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


void Display::plot_vectors(){
    // -- actual action
    glUseProgram(program);
    glBindVertexArray(VAO);
    glLineWidth(4);
    glDrawArrays(GL_LINES, 0, 4);
}


void Display::poll_events(){
    // -- post
    glfwSwapBuffers(window);
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

