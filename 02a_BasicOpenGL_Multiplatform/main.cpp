#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

GLuint programId;
GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;

static GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
static GLuint createShader(const GLenum shaderType, const std::string shaderSource);

static const GLfloat vertexPositionData[] = {
    -0.9f, -0.9f,
     0.9f, -0.9f,
    -0.9f,  0.9f,
     0.9f,  0.9f
};
static const GLushort indexData[] = { 0, 1, 2, 3 };

static GLuint createGeometry(void) {
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

static void update(void) {
    int milliseconds = glutGet(GLUT_ELAPSED_TIME);

    // TODO: Some update

    glutPostRedisplay();
}

static void render(void) {
   // clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	// draw our triangle (the positionBuffer is the active buffer)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// swap the buffers to make the one we drew to visible
   glutSwapBuffers();
}

static void reshape(int width, int height) {
    // if using perpsective projection, update projection matrix
    glViewport(0, 0, width, height);
}

static void drag(int x, int y) {
}

static void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    }
}

static void keyboard(unsigned char key, int x, int y) {
    std::cout << "Key pressed: " << key << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("CSCI 3090u Base OpenGL");
    glutIdleFunc(&update);
    glutDisplayFunc(&render);
    glutReshapeFunc(&reshape);
    glutMotionFunc(&drag);
    glutMouseFunc(&mouse);
    glutKeyboardFunc(&keyboard);

    glewInit();
    if (!GLEW_VERSION_2_0) {
        std::cerr << "OpenGL 2.0 not available" << std::endl;
        return 1;
    }
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

    programId = createGeometry();

    glutMainLoop();

    return 0;
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
