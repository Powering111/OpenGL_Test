#pragma once
#include "Sprite.h"
#include "Texture.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class SpriteRenderer
{
private:
	unsigned int VAO, position_VBO, texture_VBO;
	glm::mat4 projection;
	float width, height;
protected:
	Shader shader;
public:
	SpriteRenderer(Shader &shader, float width, float height);
	void draw_center(Sprite &sprite, glm::vec3 position, glm::vec3 scale=glm::vec3(1.0f), float rotation = 0,float fill=1);
	void draw_rect(Sprite &sprite, glm::vec3 position, glm::vec3 scale=glm::vec3(1.0f), float rotation = 0,float fill=1);
	void setViewport(float width, float height);
};
