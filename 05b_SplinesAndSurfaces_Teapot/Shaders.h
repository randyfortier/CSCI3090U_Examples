#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

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

		delete[] errorMessage;

		glDeleteShader(shaderId);

		return 0;
	}

	return shaderId;
}

static GLuint createShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& tessControlShaderFilename="", const std::string& tessEvalShaderFilename="") {
	// create and compile a shader for each
  std::cout << "Compiling vertex shader..." << std::endl;
	GLuint vShaderId = createShader(GL_VERTEX_SHADER, vertexShaderFilename);
  std::cout << "Compiling fragment shader..." << std::endl;
	GLuint fShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
	GLuint tcShaderId = -1;
	if (!tessControlShaderFilename.empty()) {
    std::cout << "Compiling tessellation control shader..." << std::endl;
		tcShaderId = createShader(GL_TESS_CONTROL_SHADER, tessControlShaderFilename);
	}
	GLuint teShaderId = -1;
	if (!tessEvalShaderFilename.empty()) {
    std::cout << "Compiling tessellation evaluation shader..." << std::endl;
		teShaderId = createShader(GL_TESS_EVALUATION_SHADER, tessEvalShaderFilename);
	}
	// create and link the shaders into a program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	if (tcShaderId >= 0) {
		glAttachShader(programId, tcShaderId);
	}
	if (teShaderId >= 0) {
		glAttachShader(programId, teShaderId);
	}
	glLinkProgram(programId);
	glValidateProgram(programId);

	// delete the shaders
	glDetachShader(programId, vShaderId);
	glDetachShader(programId, fShaderId);
	glDeleteShader(vShaderId);
	glDeleteShader(fShaderId);
	if (tcShaderId >= 0) {
		glDetachShader(programId, tcShaderId);
		glDeleteShader(tcShaderId);
	}
	if (teShaderId >= 0) {
		glDetachShader(programId, teShaderId);
		glDeleteShader(teShaderId);
	}

	return programId;
}

void dumpProgram(int programId, const std::string description) {
	char name[256];
	GLsizei length;
	GLint size;
	GLenum type;
	int uniforms;
	int attributes;
	int shaders;
	int i;

	std::cout << "Information for shader: " << description << std::endl;

	if(!glIsProgram(programId)) {
		std::cerr << "Error: not a valid shader program" << std::endl;
		return;
	}

	glGetProgramiv(programId, GL_ATTACHED_SHADERS, &shaders);
	std::cout << "Number of shaders: " << shaders << std::endl;

	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniforms);
  std::cout << "Number of uniforms: " << uniforms << std::endl;
	for(int i = 0; i < uniforms; i++) {
		glGetActiveUniform(programId, i, 256, &length ,&size ,&type, name);
		std::cout << "  name: " << name << std::endl;
	}
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &attributes);
  std::cout << "Number of attributes: " << attributes << std::endl;
	for(int i = 0; i < attributes; i++) {
		glGetActiveAttrib(programId, i, 256, &length, &size, &type, name);
    std::cout << "  name: " << name << std::endl;
	}
}
