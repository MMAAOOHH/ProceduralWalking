#pragma once

#include "game_object.h"
#include "Mouse.h"
#include "Window.h"
#include "renderer.h"

struct Engine
{
	Engine(GLuint width, GLuint height);
	~Engine();
	GLuint width, height;

	std::unique_ptr<Window> window;
	std::unique_ptr <SpriteRenderer> renderer;
	std::unique_ptr <Camera> camera;

	//research unique ptr, shared ptr
	Shader* quad_shader;
	Shader* circ_shader;
	Shader* ghost_shader;

	Material* quad_mat;
	Material* circ_mat;
	Material* circ_mat2;

	Texture* texture_a;
	Texture* texture_b;

	GLfloat delta_time = 0.0f;

	void init();
	void handle_input();
	void update();
	void render();
	void draw_circle(glm::vec2 pos, GLfloat size);

	std::vector<std::shared_ptr<GameObject>> objects;
	std::shared_ptr<GameObject> add_game_object();
	std::shared_ptr<GameObject> add_circle_object(GLfloat scale);
	//void remove_game_object(const std::shared_ptr<GameObject>& go);
};
