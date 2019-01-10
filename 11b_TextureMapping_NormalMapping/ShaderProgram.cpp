#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
	this->vertexShaderId = -1;
	this->fragmentShaderId = -1;
	this->programId = -1;
}

std::string ShaderProgram::getVertexShaderCode() { return this->vertexShaderCode; }
std::string ShaderProgram::getFragmentShaderCode() { return this->fragmentShaderCode; }
GLuint ShaderProgram::getVertexShaderId() { return this->vertexShaderId; }
GLuint ShaderProgram::getFragmentShaderId() { return this->fragmentShaderId; }
GLuint ShaderProgram::getProgramId() { return this->programId; }

GLuint ShaderProgram::loadShaders(const std::string vertexShaderFilename, const std::string fragmentShaderFilename) {
	// create and compile a shader for each
	this->vertexShaderId = this->loadShader(GL_VERTEX_SHADER, vertexShaderFilename);
	this->fragmentShaderId = this->loadShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	// create and link the shaders into a program
	this->programId = glCreateProgram();
	glAttachShader(this->programId, this->vertexShaderId);
	glAttachShader(this->programId, this->fragmentShaderId);
	glLinkProgram(this->programId);
	glValidateProgram(this->programId);

	// delete the shaders
	glDetachShader(this->programId, this->vertexShaderId);
	glDetachShader(this->programId, this->fragmentShaderId);
	glDeleteShader(this->vertexShaderId);
	glDeleteShader(this->fragmentShaderId);

	return this->programId;
}

GLuint ShaderProgram::loadShader(const GLenum shaderType, const std::string shaderFilename) {
	// load the contents of the specified text file
	std::ifstream fileIn(shaderFilename);

	if (!fileIn.is_open()) {
		return -1;
	}

	// load the shader code into a string
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
