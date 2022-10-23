#include "Sprite.h"
Sprite::Sprite(const char* imagePath, unsigned int div) 
	: div(div), index(0) {
	texture.load(imagePath);
};