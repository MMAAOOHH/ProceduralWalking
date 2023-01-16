#pragma once

#include "glad/glad.h"
#include "shader.h"
#include "texture.h"

class Material
{
	static GLuint id_gen;
	GLuint tex_unit_;

public:
	Texture* texture;
	Shader* shader;
	GLuint id;
	glm::vec3 color = {1,1,1};
	// Uniforms

	Material(Texture* texture, Shader* shader, GLint tex_unit)
		: texture(texture), shader(shader), id(id_gen++), tex_unit_(tex_unit)
	{}

	~Material()
	{}

	void compile(); 
	void bind(); 
};
