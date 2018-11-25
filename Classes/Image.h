#pragma once

#include <vector>
#include <iostream>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

class Image {
public:
	GLuint textureID;
	unsigned char* data;
	int
		width,
		height,
		channels;

	Image();
	~Image();

	void Bind(std::vector<std::uint8_t> data, int width, int height, bool useAlpha);
	void Load(const GLchar* filePath, bool useAlpha);
	void Use();
};