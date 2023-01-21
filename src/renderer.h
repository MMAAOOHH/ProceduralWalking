#pragma once

#include <glad/glad.h>
#include <vector>
#include "material.h"
#include "rect.h"

struct Drawable;

class SpriteRenderer
{
	GLuint vao_;
public:
	SpriteRenderer();
	~SpriteRenderer();

	void draw(const Drawable& drawable_struct, const glm::mat4& view);
};

class Renderer
{
	GLuint vbo_, vao_, attrib_size_;
	std::vector<GLfloat> buffer_;
	Material* current_material_;
	GLuint max_sprites_;

public:
	Renderer(std::vector<GLuint> attributes, GLuint max_sprites);
	~Renderer();

	void begin();
	void end();
	void draw(Drawable& drawable_struct);
	void flush();
};
