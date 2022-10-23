#pragma once
#include <glad/glad.h>
class Texture
{
private:
	unsigned int ID;
	unsigned int internal_format, image_format, wrap_s, wrap_t, filter_min, filter_mag;
public:
	Texture();
	void load(const char* imagePath);
	void bind();
};

