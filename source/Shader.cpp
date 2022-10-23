#include "Shader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
std::string readfile(const char* filepath) {
	std::string result;
	try {
		std::ifstream stream(filepath);

		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		result = ss.str();
	}
	catch (std::exception e) {
		std::cerr << "reading file " << filepath << " error!" << std::endl;
	}
	return result;
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	load(vertexShaderPath, fragmentShaderPath);
}

void Shader::load(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::string vertexShaderString = readfile(vertexShaderPath);
	std::string fragmentShaderString = readfile(fragmentShaderPath);
	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragmentShaderSource = fragmentShaderString.c_str();
	unsigned int vshader, fshader, shader;
	int success;
	char infoLog[1024];
	vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, &vertexShaderSource, NULL);
	glCompileShader(vshader);
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vshader, 1024, NULL, infoLog);
		std::cout << "Error while compiling vertex shader.\n" << infoLog << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fshader, 1024, NULL, infoLog);
		std::cout << "Error while compiling fragment shader.\n" << infoLog << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	shader = glCreateProgram();
	glAttachShader(shader, vshader);
	glAttachShader(shader, fshader);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "Error while linking shaders.\n" << infoLog << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	this->ID=shader;
}

void Shader::Use() {
	glUseProgram(ID);
}

void Shader::setInt(const char* name, int value) {
	glUniform1i(glGetUniformLocation(ID, name), value);
};
void Shader::setFloat(const char* name, float value) {
	glUniform1f(glGetUniformLocation(ID, name), value);
};
void Shader::setVector2f(const char* name, float x, float y) {
	glUniform2f(glGetUniformLocation(ID, name), x,y);
};
void Shader::setVector3f(const char* name, float x, float y, float z){
	glUniform3f(glGetUniformLocation(ID, name), x,y,z);
};
void Shader::setVector4f(const char* name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(ID, name), x,y,z,w);
};
void Shader::setVector2f(const char* name, glm::vec2 &value) {
	glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
};
void Shader::setVector3f(const char* name, glm::vec3& value) {
	glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
};
void Shader::setVector4f(const char* name, glm::vec4& value) {
	glUniform4f(glGetUniformLocation(ID, name), value.x,value.y,value.z,value.w);
};
void Shader::setMatrix4f(const char* name, glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(value));
};