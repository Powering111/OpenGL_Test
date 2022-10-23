#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Sprite
{
public:
	Sprite(const char* imagePath, unsigned int div=1);
	unsigned int div;
	unsigned int index;
	Texture texture;
};

