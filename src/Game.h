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

	std::shared_ptr<GameObject> r1, r2, r3, l1, l2, l3, debug_target, base, debug;
	std::shared_ptr<IKSolver> leg_r, leg_l;
	GLfloat length = 200.0f;

	glm::vec2 lerp_position_r = { 0,length * 2 };
	glm::vec2 lerp_position_l = { 0,length * 2 };
	glm::vec2 cur_pos_r = { 0,length * 2 };
	glm::vec2 cur_pos_l = { 0,length * 2 };

	glm::vec2 step_target_position = { 0,0 };
	GLfloat step_speed = 20.0f;
	bool moving_right = true;

	bool step = false;

	GLfloat step_time = 0.2f;

	glm::vec2 pivot_centre{ 0,0 };
	glm::vec2 step_start{ 0,0 };

	void start() override
	{
		// Leg IK
		leg_r = std::make_shared<IKSolver>();
		leg_l = std::make_shared<IKSolver>();

		leg_r->solve(length, length, { 0,0 }, { 0 , length * 2 }, 0);
		leg_l->solve(length, length, { 0,0 }, { 0 , length * 2 }, 0);

		// Right Leg
		r1 = engine->add_game_object();
		r2 = engine->add_game_object();
		r3 = engine->add_game_object();

		// Left Leg
		l1 = engine->add_game_object();
		l2 = engine->add_game_object();
		l3 = engine->add_game_object();

		debug_target = engine->add_game_object();
		base = engine->add_game_object();
		debug = engine->add_game_object();

		for (auto go : engine->objects)
		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat;
			circ->material->color = { 1,0.5,0 };
			go->drawable = *circ;
		}

		debug_target->drawable.material = engine->circ_mat2;
		debug_target->drawable.size *= 0.5f;

		base->drawable.material = engine->circ_mat2;
		base->drawable.size *= 0.5f;

		// Starting positions
		base->transform.position = { 0,60 };

		lerp_position_r = leg_r->last;
		lerp_position_l = leg_r->last;

		step_target_position = { 0 , length * 2 };
		step_target_position.x += 50;
		debug_target->transform.position = step_target_position;
	}



	GLfloat elapsed = 0.0f;
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

		// body movement
		base->transform.position += direction * 5.0f;
		if (moving_right)
			step_target_position.x = base->transform.position.x + 200;
		else
			step_target_position.x = base->transform.position.x - 200;
		


		GLfloat distance = glm::distance(base->transform.position, lerp_position_r);
		if (distance > length * 2)
		{
			// Take step
			step_start = cur_pos_r;
			pivot_centre = (step_target_position - step_start) * 0.5f + step_start;
			step = true;
		}

		if (step)
		{
			elapsed += dt;
			GLfloat frac_complete = elapsed / step_time;
			GLfloat angle = lerp(0, glm::pi<GLfloat>(), frac_complete);
			lerp_position_r = rotate(step_start, pivot_centre, angle);
			if (frac_complete >= 1.0f)
			{
				step = false;
				elapsed = 0.0f;
				lerp_position_r = step_target_position;
			}
		}



		cur_pos_r = lerp(cur_pos_r, lerp_position_r, dt * step_speed);

		leg_r->solve(length, length, base->transform.position, cur_pos_r, moving_right);
		leg_l->solve(length, length, base->transform.position, cur_pos_r, moving_right);




		debug_target->transform.position = step_target_position;

		
		r1->transform.position = leg_r->first;
		r2->transform.position = leg_r->second;
		r3->transform.position = leg_r->last;

		l1->transform.position = leg_l->first;
		l2->transform.position = leg_l->second;
		l3->transform.position = leg_l->last;

	}
};


