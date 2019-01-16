#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow * window;

static GLuint createShader(const GLenum type, const std::string shaderSource);
GLuint createGeometry();
void render();
static GLuint createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);

int main(void) {
	// initialize GLFW
	if (!glfwInit()) {
		return -1;
	}

	// create a window
	window = glfwCreateWindow(800, 600, "CSCI 3090u", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// make an OpenGL context
	glfwMakeContextCurrent(window);

	// initialize GLEW (must be after creating the OpenGL context, but before calling any OpenGL functions)
	GLenum error = glewInit();

	if (GLEW_OK != error) {
		// error initializing GLEW
		std::cerr << "Error: " << glewGetErrorString(error) << std::endl;
	} else {
		// GLEW is initialized
		std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;
	}

	GLuint programId = createGeometry();

	// loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		render();

		// handle events (later)
		glfwPollEvents();
	}

	glDeleteProgram(programId);

	glfwTerminate();

	return 0;
}

void render() {
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	// draw our triangle (the positionBuffer is the active buffer)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// swap the buffers to make the one we drew to visible
	glfwSwapBuffers(window);
}

GLuint createGeometry() {
	// set the background colour
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Add the geometry

	// create a buffer to store the vertex positions in graphics memory
	GLuint positionBufferId;
	glGenBuffers(1, &positionBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);

	// the positions
	float positions[6] = {
		-0.5f, -0.5f,  // a
		 0.0f,  0.5f,  // b
		 0.5f, -0.5f   // c
	};

	// pass the data to OpenGL (graphics memory)
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// create and enable a vertex attribute array
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// Add the shader code
	std::string vertexShaderCode = ""
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main() {\n"
		"   gl_Position = position;\n"
		"}\n";

	std::string fragmentShaderCode = ""
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 colour;\n"
		"\n"
		"void main() {\n"
		"   colour = vec4(1.0, 0.3, 0.0, 1.0);\n"
		"}\n";

	// create a program from our shaders
	GLuint programId = createShaderProgram(vertexShaderCode, fragmentShaderCode);
	glUseProgram(programId); // make it active

	return programId;
}

static GLuint createShader(const GLenum shaderType, const std::string shaderSource) {
	const char* sourceCode = shaderSource.c_str();

	// create a shader with the specified source code
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &sourceCode, nullptr);

	// compile the shader
	glCompileShader(shaderId);

	// check if there were any compilation errors
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int errorLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLength);
		char *errorMessage = new char[errorLength];

		glGetShaderInfoLog(shaderId, errorLength, &errorLength, errorMessage);
		std::cout << "Shader compilation failed: " << errorMessage << std::endl;

		delete errorMessage;

		glDeleteShader(shaderId);

		return 0;
	}

	return shaderId;
}

static GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
	// create and compile a shader for each
	GLuint vShaderId = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	// create and link the shaders into a program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, fShaderId);
	glAttachShader(programId, vShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	// delete the shaders
	glDetachShader(programId, vShaderId);
	glDetachShader(programId, fShaderId);
	glDeleteShader(vShaderId);
	glDeleteShader(fShaderId);

	return programId;
}
