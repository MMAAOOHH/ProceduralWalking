#include "material.h"

GLuint Material::id_gen = 0;

void Material::compile()
{
	shader->use();
	shader->set_int("image", tex_unit_);
	shader->set_vec3f("u_color", color);
	
}

void Material::bind()
{
	glActiveTexture(GL_TEXTURE0 + tex_unit_);
	texture->bind();
}
