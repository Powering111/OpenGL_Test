#include "SpriteRenderer.h"
static float quad[] = {
	//  pos     
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f,  0.5f,
	-0.5f,  0.5f,
	0.5f, -0.5f,
	0.5f,  0.5f,
};
static float texture_pos[] = {
	0,0,
	1,0,
	0,1,
	0,1,
	1,0,
	1,1
};
SpriteRenderer::SpriteRenderer(Shader& shader,float width, float height)
	: width(width),height(height),shader(shader) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &position_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glGenBuffers(1, &texture_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, texture_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_pos), texture_pos, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	setViewport(width, height);
}

void SpriteRenderer::draw_center(Sprite& sprite, glm::vec3 position, glm::vec3 scale,float rotation,float fill) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));
	float w = 1.0 / sprite.div;
	float h = 1.0;
	float x = w * (sprite.index%sprite.div);
	float y = 0;
	float vertex[] = {
		-0.5f, -0.5f,
		- 0.5f + fill, -0.5f,
		-0.5f,  0.5f,
		-0.5f,  0.5f,
		-0.5f+fill, -0.5f,
		-0.5f+fill,  0.5f,
	};
	float texture[] = {
		x, y,
		x + w*fill, y,
		x, y + h,
		x, y + h,
		x + w*fill, y,
		x + w*fill, y + h
	};
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);

	glBindBuffer(GL_ARRAY_BUFFER , texture_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texture), texture);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	sprite.texture.bind();
	shader.Use();
	shader.setMatrix4f("model", model);
	shader.setMatrix4f("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SpriteRenderer::draw_rect(Sprite& sprite, glm::vec3 position, glm::vec3 scale, float rotation, float fill) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));
	float w = 1.0 / sprite.div;
	float h = 1.0;
	float x = w * (sprite.index % sprite.div);
	float y = 0;
	float vertex[] = {
		0, 0,
		fill, 0,
		0,  1.0f,
		0,  1.0f,
		fill, 0,
		fill,  1.0f,
	};
	float texture[] = {
		x, y,
		x + w * fill, y,
		x, y + h,
		x, y + h,
		x + w * fill, y,
		x + w * fill, y + h
	};
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);

	glBindBuffer(GL_ARRAY_BUFFER, texture_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texture), texture);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	sprite.texture.bind();
	shader.Use();
	shader.setMatrix4f("model", model);
	shader.setMatrix4f("projection", projection);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SpriteRenderer::setViewport(float width, float height) {
	this->projection = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
}