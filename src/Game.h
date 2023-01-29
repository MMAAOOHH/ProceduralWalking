#pragma once

#include <memory>
#include <glad/glad.h>

#include "Engine.h"
#include "Keyboard.h"
#include "IKSolver.h"

struct Game
{
	Game() = default;
	virtual ~Game() = default;

	virtual void run() = 0;
	virtual void start() = 0;
	virtual void update(GLfloat dt) = 0;
};

struct Prototype : Game
{
	std::unique_ptr<Engine> engine;

	Prototype(GLuint width, GLuint height)
	{
		engine = std::make_unique<Engine>(width, height);
		engine->init();
	}
	~Prototype() override = default;

	void run() override
	{
		while (engine->window->is_open())
		{
			engine->handle_input();
			update(engine->delta_time);
			engine->update();
			engine->render();
		}
	}

	std::shared_ptr<GameObject> p1, p2, p3, handle, base;
	glm::vec2 v1 = { 0,0 };
	glm::vec2 v2 = { 0,0 };
	glm::vec2 v3 = { 0,0 };

	// IK Testing
	std::shared_ptr<IKSolver> leg;
	GLfloat length = 200.0f;
	

	void start() override
	{
		p1 = engine->add_game_object();
		p2 = engine->add_game_object();
		p3 = engine->add_game_object();
		handle = engine->add_game_object();
		base = engine->add_game_object();
		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat;
			circ->material->color = { 1,0.5,0 };
			p1->drawable = *circ;
		}
		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat;
			p2->drawable = *circ;
		}
		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat;
			p3->drawable = *circ;
		}

		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat2;
			circ->material->color = { 1,1,0 };
			handle->drawable = *circ;
			handle->drawable.size = glm::vec2(32.0f);
		}

		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat2;
			circ->material->color = { 1,1,0 };
			base->drawable = *circ;
			base->drawable.size = glm::vec2(32.0f);
		}


		leg = std::make_shared<IKSolver>();

		leg->solve(length, length, { 0,0 }, { 0 , length * 2}, 0);
		handle->transform.position = leg->last;
		base->transform.position = leg->first;

		lerp_position = leg->last;
		target_position = leg->last;
		target_position.x += 50;
	}

	GLfloat elapsed = 0;
	glm::vec2 lerp_position = { 0,length * 2 };
	glm::vec2 cur_position = { 0,length * 2 };
	glm::vec2 target_position = { 0,0 };
	GLfloat step_speed = 20.0f;
	bool moving_right = true;

	void update(GLfloat dt) override
	{
		glm::vec2 direction = { 0,0 };

		if (Keyboard::key(GLFW_KEY_RIGHT))
		{
			direction.x = 1;
			moving_right = true;
		}
		if (Keyboard::key(GLFW_KEY_LEFT))
		{
			direction.x = -1;
			moving_right = false;
		}
		if (Keyboard::key(GLFW_KEY_UP))
			direction.y = -1;
		if (Keyboard::key(GLFW_KEY_DOWN))
			direction.y = 1;

		// body movement
		base->transform.position += direction * 10.0f;
		if (moving_right)
			target_position.x = base->transform.position.x + 200;
		else
		{
			target_position.x = base->transform.position.x - 200;
		}
		
		// distance between base
		GLfloat distance = glm::distance(base->transform.position, lerp_position);
		if (distance > length * 2)
			lerp_position = target_position;

		cur_position = lerp(cur_position, lerp_position, dt * step_speed);



		leg->solve(length, length, base->transform.position, cur_position, moving_right);

		handle->transform.position = target_position;

		p1->transform.position = leg->first;
		p2->transform.position = lerp(p2->transform.position, leg->second, 10.0f* dt);
		p3->transform.position = leg->last;

				/*
		// FK-testing
		distance = calculate_distance(v1, v3);
		rotation_angle = calculate_angle(v1, v3);

		glm::clamp(rotation_angle, 0.0f, glm::two_pi<GLfloat>());

		// inverse cosine, distance / lenght * 2
		GLfloat angle = glm::acos(distance / (length * 2));

		// Calculate "Elbow"
		// ----------------------
		// x = angle + cosine of the radian * length, y = sine of the angle + radian * length
		v2 = { (glm::cos(-angle + rotation_angle)) * length, glm::sin(-angle + rotation_angle) * length };

		// Calculate End Effector, "ankle"
		// ----------------------
		// radian 90 degrees over pi
		// x = cosine of the radian * distance, y = sine of the radian * distance
		//v3 = { glm::cos(rotation_angle) * distance, glm::sin(rotation_angle) * distance };
		*/
	}
};
