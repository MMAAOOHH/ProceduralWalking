#pragma once
#include <bitset>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Window
{
public:

	Window(const char* name, GLuint width, GLuint height);
	~Window();

	void clear();
	void update();
	void process_events();
	bool is_open();

	GLuint width, height;
private:

	void init(const char* name, GLuint width, GLuint height);

	GLFWwindow* window_;
	std::bitset<8> buttons_;

	const char* name_;
	glm::vec3 bg_color_ = { 0.1f,0.0f,0.1f };
};
