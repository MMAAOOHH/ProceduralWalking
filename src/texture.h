#pragma once
#include <glad/glad.h>
#include <string>

class Texture
{
	GLuint id_;
public:
	// image dimensions
	GLuint width, height;

	// texture format
	GLuint internal_format;
	GLuint image_format;

	// texture configuration
	GLuint wrap_s;
	GLuint wrap_t;
	GLuint filter_min;
	GLuint filter_mag;

	Texture(GLuint internal_format, GLuint image_format, GLuint wrap_s, GLuint wrap_t, GLuint filter_min, GLuint filter_mag);
	Texture(GLuint format);
	Texture();
	~Texture();

	void load(const GLchar* tex_file_name);
	void bind();
};