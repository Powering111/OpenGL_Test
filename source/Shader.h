#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
class Shader
{
private:
	unsigned int ID;
public:
	Shader() : ID(0) {};
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void load(const char* vertexShaderPath, const char* fragmentShaderPath);

	void Use();
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setVector2f(const char* name, float x, float y);
	void setVector3f(const char* name, float x, float y, float z);
	void setVector4f(const char* name, float x, float y, float z, float w);
	void setVector2f(const char* name, glm::vec2 &value);
	void setVector3f(const char* name, glm::vec3 &value);
	void setVector4f(const char* name, glm::vec4 &value);
	void setMatrix4f(const char* name, glm::mat4 &value);

};