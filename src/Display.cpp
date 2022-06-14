#include"Display.h"

// -- This Class is a abomination.
Display::Display(Camera* cam){  
	

	// -- Set Camera reference.
	camera = cam;

	// -- setting projection matrix
	projectionMat4 = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 15.0f);

	// -- Initialization GLFW.
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "VFV", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 800, 600);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// -- Read in shaders.
	std::string VSSinput = readShaderCode("vertexshader.glsl");
	std::string FSSinput = readShaderCode("fragmentshader.glsl");
	std::string TVSSinput = readShaderCode("textshader.glsl");
	std::string TFSSinput = readShaderCode("textfragmentshader.glsl");
	const char* VSS = VSSinput.c_str();
	const char* FSS = FSSinput.c_str();
	const char* TVSS = TVSSinput.c_str();
	const char* TFSS = TFSSinput.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

	GLuint textVertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint textFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	textProgram = glCreateProgram();

	// -- Compilation and linking.
	glShaderSource(vertexShader, 1, &VSS, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, &FSS, NULL);
	glCompileShader(fragmentShader);

	glShaderSource(textVertexShader, 1, &TVSS, NULL);
	glCompileShader(textVertexShader);

	glShaderSource(textFragmentShader, 1, &TFSS, NULL);
	glCompileShader(textFragmentShader);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glAttachShader(textProgram, textVertexShader);
	glAttachShader(textProgram, textFragmentShader);
	glLinkProgram(textProgram);


	// -- Delete shaders after linking.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(textVertexShader);
	glDeleteShader(textFragmentShader);


	// -- freetype initialization
	FT_Library freetype;
	FT_Face face;

	FT_Init_FreeType(&freetype);
	FT_New_Face(freetype, "C:/Windows/Fonts/arial.ttf", 0, &face);
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 32; c < 128; c++) {
		// -- Load "c" as a font glyph.
		FT_Load_Char(face, c, FT_LOAD_RENDER);

		// -- Create a texture around "c".
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// -- Set textures options.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// -- Store "c".
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(freetype);

	glGenVertexArrays(1, &TVAO);
	glGenBuffers(1, &TVBO);
	glBindVertexArray(TVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void Display::drawGraph(Graph* graph) {
	glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// -- Draw Arrows.
	for (Arrow* arrow : graph->getArrows()) {
		drawArrow(arrow);
	}

	// -- Draw the Graphs borders.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, graph->getVertexDataSizeInBytes(), graph->getVertexData(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	// -- matrix operations
	glm::mat4 transform = projectionMat4 * camera->getWorldToViewMatrix() * graph->getTranslationMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "transformMat4"), 1, GL_FALSE, &transform[0][0]);

	glUseProgram(program);
	glBindVertexArray(VAO);
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, 24);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// -- Draw text
	for (TextLabel* text : graph->getText()) {
		drawText(text);
	}

	// -- Swap front and back buffers
	glfwSwapBuffers(window);
}


void Display::drawArrow(Arrow* arrow) {
	// -- Draw the Graphs vectors
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glUseProgram(program);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, arrow->getVertexDataSizeInBytes(), arrow->getVertexData(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrow->getDataIndicesSizeInBytes(), arrow->getDataIndices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	// -- matrix operations
	glm::mat4 transform = projectionMat4 * camera->getWorldToViewMatrix() * arrow->getTranslationMatrix() * arrow->getRotationMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "transformMat4"), 1, GL_FALSE, &transform[0][0]);


	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_SHORT, 0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void Display::drawText(TextLabel* textLabel) {
	glUseProgram(textProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(TVAO);

	// -- set uniforms
	glUniform3f(glGetUniformLocation(textProgram, "textColor"), textLabel->color.r, textLabel->color.g, textLabel->color.b);

	glm::mat4 transform = projectionMat4 * camera->getWorldToViewMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(textProgram, "testMat4"), 1, GL_FALSE, &transform[0][0]);

	string text = textLabel->text;

	float span = 0.0f;
	for (int i = 0; i < text.size(); i++) {
		span += (Characters[text[i]].Advance >> 6);
	}

	// -- Pixel world locations of TextLabel position.
	float x = ( (textLabel->origin.x - textLabel->width / 2.0f) + 1.0f) * (600.0f / 2.0f);
	float y = (textLabel->origin.y + 1.0f) * (800.0f / 2.0f);
	float z = textLabel->origin.z;

	// -- Calculate how much to scale the text to fit the label.
	float scale = (textLabel->width / 2.0f * 600) / span;
	


	for (int i = 0; i < text.size(); i++) {
		Character ch = Characters[text[i]];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		

		// -- Normalized position to -1.0 to 1.0
		xpos = ((2.0f * xpos) / 600.0f) - 1.0f;
		ypos = ((2.0f * ypos) / 800.0f) - 1.0f;
		float w = (ch.Size.x * scale) / (600.0f / 2.0f);
		float h = (ch.Size.y * scale) / (800.0f / 2.0f);



		// update VBO for each character
		GLfloat vertices[6][5] = 
		{	
			{ xpos,     ypos + h,	z,		0.0f, 0.0f },
			{ xpos,     ypos,       z,		0.0f, 1.0f },
			{ xpos + w, ypos,       z,		1.0f, 1.0f },
			{ xpos,     ypos + h,   z,		0.0f, 0.0f },
			{ xpos + w, ypos,       z,		1.0f, 1.0f },
			{ xpos + w, ypos + h,   z,		1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, TVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * 5, vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	glDeleteVertexArrays(1, &TVAO);
	glDeleteBuffers(1, &TVBO);
	glDeleteProgram(program);
	glDeleteProgram(textProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
}

string Display::readShaderCode(const char* filename) {
	string shader, line;
	ifstream inputFile(filename);
	if (!inputFile.good()) {
		cout << "FAILED TO LOAD FILE: " << filename << " IN readShaderCode()" << endl;
		exit();
	}
	while (getline(inputFile, line)) {
		shader.append(line);
		shader.append("\n");
	}
	return shader;
}
