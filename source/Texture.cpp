#include "Texture.h"
#include <iostream>

#include <stb_image.h>
Texture::Texture() : ID(0), internal_format(GL_RGBA), image_format(GL_RGBA), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_mag(GL_NEAREST) {
	glGenTextures(1, &ID);
}

void Texture::load(const char* imagePath) {
	stbi_set_flip_vertically_on_load(true);
	int imgwidth, imgheight, nrChannels;
	unsigned char* data = stbi_load(imagePath, &imgwidth, &imgheight, &nrChannels, 0);
	if (!data) {
		std::cerr << "texture load failure: " << imagePath << std::endl;
		return;
	}
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, imgwidth, imgheight, 0, image_format, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, ID);
}