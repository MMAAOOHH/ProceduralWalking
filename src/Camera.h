#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

struct Camera
{
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 view_size = glm::vec2(800, 800);
	glm::vec2 offset = glm::vec2(0.5);
	GLfloat zoom = 1.0f;
	GLfloat zoom_sensitivity = 15.f;
	GLfloat pan_speed = 50.0f;

	glm::mat4 get_orthographic_projection()
	{
		return glm::ortho(0.0f, view_size.x, view_size.y, 0.0f, -1.0f, 1.0f);
	}
	glm::mat4 transform_view()
	{
		glm::mat4 view = glm::mat4(1);
		view = glm::translate(view, glm::vec3(view_size * offset, 0.0f));
		view = glm::scale(view, glm::vec3(glm::vec2(zoom), 0.0f));
		view = glm::translate(view, glm::vec3(-position, 0.0f));

		return view;
	}
};