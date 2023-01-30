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

	// Joints and body parts
	std::shared_ptr<GameObject> r1, r2, r3, l1, l2, l3, r_upper, r_lower, l_upper, l_lower, body, head, eye1, eye2;

	std::shared_ptr<IKSolver> leg_r, leg_l;

	glm::vec2 root, r_base, l_base;
	glm::vec2 lerp_position_r = { 0,length * 2 };
	glm::vec2 lerp_position_l = { 0,length * 2 };
	glm::vec2 cur_pos_r = { 0,length * 2 };
	glm::vec2 cur_pos_l = { 0,length * 2 };

	GLfloat length = 200.0f;
	GLfloat step_size = 200.0f;
	GLfloat step_distance = 200.0f;
	GLfloat step_speed = 20.0f;


	glm::vec2 step_target_position = { 0,0 };
	glm::vec2 pivot_centre_r{ 0,0 };
	glm::vec2 pivot_centre_l{ 0,0 };
	glm::vec2 step_start{ 0,0 };

	bool moving_right = true;
	bool r_step = false;
	bool l_step = false;
	

	void start() override
	{
		// Leg IK
		leg_r = std::make_shared<IKSolver>();
		leg_l = std::make_shared<IKSolver>();

		// Right Leg
		r1 = engine->add_game_object();
		r2 = engine->add_game_object();
		r3 = engine->add_game_object();
		r_upper = engine->add_game_object();
		r_lower = engine->add_game_object();

		// Left Leg
		l1 = engine->add_game_object();
		l2 = engine->add_game_object();
		l3 = engine->add_game_object();
		l_upper = engine->add_game_object();
		l_lower = engine->add_game_object();


		body = engine->add_game_object();
		head = engine->add_game_object();
		eye1 = engine->add_game_object();
		eye2 = engine->add_game_object();



		// Creates drawables
		for (auto go : engine->objects)
		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat;
			circ->material->color = { 1,0.5,0 };
			go->drawable = *circ;
		}

		// Head
		{
			auto quad = std::make_shared<struct Drawable>();
			quad->material = engine->quad_mat;
			quad->transform_origin = Drawable::centered;
			quad->size *= 3;

			head->drawable = *quad;
		}

		// Body
		{
			auto quad = std::make_shared<struct Drawable>();
			quad->material = engine->quad_mat;
			quad->material->color = { 1,0.5,0 };
			quad->transform_origin = Drawable::bottom_middle;
			quad->size.y *= 4;

			body->drawable = *quad;
		}


		// Limb segments
		{
			auto quad = std::make_shared<struct Drawable>();
			quad->material = engine->quad_mat;
			quad->transform_origin = Drawable::center_left;
			quad->size.x = length;
			quad->size.y *= 0.7f;

			r_upper->drawable = *quad;
		}
		{
			auto quad = std::make_shared<struct Drawable>();
			quad->material = engine->quad_mat;
			quad->transform_origin = Drawable::center_left;
			quad->size.x = length;
			quad->size.y *= 0.7f;

			r_lower->drawable = *quad;
		}

		{
			auto quad = std::make_shared<struct Drawable>();
			quad->material = engine->quad_mat;
			quad->transform_origin = Drawable::center_left;
			quad->size.x = length;
			quad->size.y *= 0.7f;

			l_upper->drawable = *quad;
		}
		{
			auto quad = std::make_shared<struct Drawable>();
			quad->material = engine->quad_mat;
			quad->transform_origin = Drawable::center_left;
			quad->size.x = length;
			quad->size.y *= 0.7f;

			l_lower->drawable = *quad;
		}



		// eyes
		{
			eye1->drawable.size *= 0.5f;
			eye1->drawable.material = engine->circ_mat2;
			eye1->drawable.material->color = { 0.1f,0.0f,0.1f };

			eye2->drawable.size *= 0.5f;
			eye2->drawable.material = engine->circ_mat2;
		}


		root = { 0,20 };


		// Starting positions
		leg_r->solve(length, length, { 0,0 }, { 0 , length * 2 }, 0);
		leg_l->solve(length, length, { 0,0 }, { 0 , length * 2 }, 0);

		r_base = leg_r->first;
		l_base = leg_l->first;

		step_target_position = { 0 , length * 2 };
		step_target_position.x += 50;

		lerp_position_r = leg_r->last;
		lerp_position_l = leg_l->last;

		pivot_centre_r.y = length * 2;
		pivot_centre_l.y = length * 2;



	}

	glm::vec2 rotated{ 0,0 };

	glm::vec2 eye_offset = { 40, 0 };
	glm::vec2 head_offset = { 0, -100};

	GLfloat blink = 0;
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
		{
			direction.y = -1;
		}
		if (Keyboard::key(GLFW_KEY_DOWN))
		{
			direction.y = 1;
		}


		// Positioning R foot
		if (Keyboard::key(GLFW_KEY_Q))
			lerp_position_r.x -= 1;
		if (Keyboard::key(GLFW_KEY_W))
			lerp_position_r.x += 1;
		// Position R base
		if (Keyboard::key(GLFW_KEY_E))
			r_base.x -= 1;
		if (Keyboard::key(GLFW_KEY_R))
			r_base.x += 1;

		// Positioning L foot
		if (Keyboard::key(GLFW_KEY_A))
			lerp_position_l.x -= 1;
		if (Keyboard::key(GLFW_KEY_S))
			lerp_position_l.x += 1;
		// Position R base
		if (Keyboard::key(GLFW_KEY_D))
			l_base.x -= 1;
		if (Keyboard::key(GLFW_KEY_F))
			l_base.x += 1;



		// Base movement
		// -------------
		root += direction * 10.0f;
		if (moving_right)
			step_target_position.x = root.x + step_size;
		else
			step_target_position.x = root.x - step_size;

		// Leg root following parent root
		r_base += root;
		l_base += root;
		

		// Walking
		// -------
		if (direction.x != 0)
		{
			if (check_step(root, leg_r->last))
				r_step = true;
			if (check_step(root, leg_l->last))
				l_step = true;
		}

		do_step_lifted(r_step, lerp_position_r, dt);
		do_step_lifted(l_step, lerp_position_l, dt);

		cur_pos_r = lerp(cur_pos_r, lerp_position_r, dt * step_speed);
		cur_pos_l = lerp(cur_pos_l, lerp_position_l, dt * step_speed);

		leg_r->solve(length, length, root, cur_pos_r, moving_right);
		leg_l->solve(length, length, root, cur_pos_l, moving_right);

		// Update visual
		// -------------
		body->transform.position = root;
		head->transform.position = body->transform.position;


		r1->transform.position = leg_r->first;
		r2->transform.position = leg_r->second;
		r3->transform.position = leg_r->last;

		l1->transform.position = leg_l->first;
		l2->transform.position = leg_l->second;
		l3->transform.position = leg_l->last;

		// Limbs visual
		r_upper->transform.position = leg_r->first;
		r_upper->transform.rotation = leg_r->angle1;
		r_lower->transform.position = leg_r->last;
		r_lower->transform.rotation = leg_r->angle1 + leg_r->angle2;


		l_upper->transform.position = leg_l->first;
		l_upper->transform.rotation = leg_l->angle1;
		l_lower->transform.position = leg_l->last;
		l_lower->transform.rotation = leg_l->angle1 + leg_l->angle2;
		/*
		l_upper->transform.position = leg_l->first;
		l_lower->transform.position = leg_l->second;
		update_limb_visual(*l_upper, leg_l->second);
		update_limb_visual(*l_lower, leg_l->last);
		*/

		/*
		pivot_centre_r.x = (step_target_position.x - leg_r->last.x) * 0.5f + leg_r->last.x;
		pivot_centre_l.x = (step_target_position.x - leg_l->last.x) * 0.5f + leg_l->last.x;
		*/

		// head bla
		head->transform.position.y = root.y - 200;
		head->transform.position.x = ease_lerp(head->transform.position.x, root.x + (head->drawable.size.x / 2 * direction.x), dt * 20.0f);

		eye1->transform.position.y = head->transform.position.y;
		eye2->transform.position.y = head->transform.position.y;

		eye1->transform.position = ease_lerp(eye1->transform.position, head->transform.position + eye_offset, dt * 20.0f) + direction;
		eye2->transform.position = ease_lerp(eye2->transform.position, head->transform.position - eye_offset, dt * 20.0f);


	}
	bool check_step(const glm::vec2 base_pos, const glm::vec2 lerp_pos)
	{
		GLfloat distance = glm::distance(base_pos, lerp_pos);
		return distance > length * 2;
	}

	void do_step_simple(glm::vec2& lerp_pos)
	{
		lerp_pos = step_target_position;
	}

	GLfloat elapsed = 0.0f;
	GLfloat step_heigth = 40;
	GLfloat circ_step_speed = 0.5f;

	void do_step_lifted(bool& step, glm::vec2& lerp_pos, GLfloat dt)
	{
		if (!step) return;

		elapsed += dt;
		GLfloat t = elapsed / circ_step_speed;
		if (t < 1)
		{
			lerp_pos = lerp(lerp_pos, step_target_position, t);
			lerp_pos.y -= sin(t * glm::pi<GLfloat>()) * step_heigth;
		}

		if (t >= 1.0f)
		{
				step = false;
				elapsed = 0.0f;
				lerp_pos = step_target_position;
		}
	}

	void do_step_circular(bool& step, glm::vec2& lerp_pos, GLfloat dt)
	{
		/*
		if (r_step)
		{
			elapsed += dt;
			GLfloat frac_complete = elapsed / circ_step_speed;
			GLfloat angle = lerp(0, glm::pi<GLfloat>(), frac_complete);
			lerp_position_r = rotate(step_start, pivot_centre, angle);
			if (frac_complete >= 1.0f)
			{
				r_step = false;
				elapsed = 0.0f;
				lerp_position_r = step_target_position;
			}
		}
		*/
	}


};
