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

static void createGeometry(void) {
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionData), vertexPositionData, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
}

static void update(void) {
	int milliseconds = glutGet(GLUT_ELAPSED_TIME);

	// TODO: Some update

	glutPostRedisplay();
}

static void render(void) {
	// activate our shader program
	glUseProgram(programId);

	// enable the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// configure the attribute array (the layout of the vertex buffer)
	glVertexAttribPointer(positionBufferId, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void *)0);
	glEnableVertexAttribArray(positionBufferId);

	// draw the triangle strip
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);

	// disable the attribute array
	glDisableVertexAttribArray(positionBufferId);

	// make the draw buffer to display buffer (i.e. display what we have drawn)
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

	createGeometry();
	programId = createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

	glutMainLoop();

	return 0;
}

static GLuint createShader(const GLenum shaderType, const std::string shaderFilename) {
	// load the shader source code
	std::ifstream fileIn(shaderFilename.c_str());

	if (!fileIn.is_open()) {
		return -1;
	}

	std::string shaderSource;
	std::string line;
	while (getline(fileIn, line)) {
		shaderSource.append(line);
		shaderSource.append("\n");
	}

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

static GLuint createShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
	// create and compile a shader for each
	GLuint vShaderId = createShader(GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	// create and link the shaders into a program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	// delete the shaders
	glDetachShader(programId, vShaderId);
	glDetachShader(programId, fShaderId);
	glDeleteShader(vShaderId);
	glDeleteShader(fShaderId);

	return programId;
}
