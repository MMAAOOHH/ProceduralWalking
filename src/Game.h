#include <memory>
#include <glad/glad.h>

#include "Engine.h"
#include "Keyboard.h"


class IKSolver
{
public:
	glm::vec2 first, second, last;

private:
	const GLfloat pi = glm::pi<GLfloat>();

public:
	void solve(GLfloat l1, GLfloat l2, glm::vec2 base, glm::vec2 target)
	{

		glm::vec2 end_effector = calculate_end_effector(base, target); 
		end_effector = clamp_distance(end_effector, l1, l2);

		GLfloat effector_vector_angle = atan2(end_effector.y, end_effector.x);
		effector_vector_angle = glm::clamp(effector_vector_angle, 0.0f, glm::two_pi<GLfloat>());

		// Calculate angles using the Law of Cosines
		const GLfloat effector_squared = end_effector.x * end_effector.x + end_effector.y * end_effector.y;

		GLfloat angle1 = acos((l1 * l1 - l2 * l2 + effector_squared) / (2.0f * l1 * sqrt(effector_squared))) + effector_vector_angle;
		if (std::isnan(angle1))
			angle1 = 0.0f;

		GLfloat angle2 = acos((l1 * l1 + l2 * l2 - effector_squared) / (2.0f * l1 * l2));
		if (std::isnan(angle1))
			angle1 = 0.0f;


		// Update positions
		first = base;
		second = first + glm::vec2(cos(angle1) * l1, sin(angle1) * l1);

		GLfloat sum1 = -angle2 - angle1;
		GLfloat sum2 = (2 * pi - angle2 - angle1);
		GLfloat x, y;

		x = cos(pi- (-angle2 - angle1)) * l2;
		y = sin(pi - (2 * pi - angle2 - angle1)) * l2;

		last = second + glm::vec2(x, y);

	}

private:
	glm::vec2 calculate_end_effector(glm::vec2 base, glm::vec2 target)
	{
		return target - base;
	}

	glm::vec2 clamp_distance(glm::vec2 end_effector, float l1, float l2)
	{
		float distance_clamped = std::max(std::abs(l1 - l2), std::min(l1 + l2, glm::length(end_effector)));
		return end_effector * distance_clamped / glm::length(end_effector);
	}

	void update_positions()
	{

		glm::vec2 v1, v2, v3, end_effector, target;
		float angle, angle1, angle2, angle3;
		float distance_clamped;
		float length1, length2;
		float pi = glm::pi<float>();
		float effector_magnitude, effector_angle;



		end_effector = target - v1;
		effector_magnitude = end_effector.length();
		effector_angle = atan2(end_effector.x, end_effector.y);
		distance_clamped = std::max(std::abs(length1 - length2), std::min(length1 + length2, glm::length(end_effector)));


		angle1 = acos((length2 * length2) - (length1 - length1) + effector_magnitude);

		v2 = v1 + cos(angle1) * length1, sin(angle1) * length1;
		v3 = v2 + cos(pi - (-angle2 - angle1)) * length2, sin(pi - (pi * 2) - angle2 - angle1 * length2);

	}
};

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

		leg->solve(length, length, { 0,0 }, { 0 , length * 2});
		handle->transform.position = leg->last;
		base->transform.position = leg->first;
	}

	GLfloat elapsed = 0;

	void update(GLfloat dt) override
	{
		glm::vec2 direction1 = { 0,0 };
		glm::vec2 direction2 = { 0,0 };

		if (Keyboard::key(GLFW_KEY_RIGHT))
			direction1.x += 1;
		if (Keyboard::key(GLFW_KEY_LEFT))
			direction1.x -= 1;
		if (Keyboard::key(GLFW_KEY_UP))
			direction1.y -= 1;
		if (Keyboard::key(GLFW_KEY_DOWN))
			direction1.y += 1;

		if (Keyboard::key(GLFW_KEY_D))
			direction2.x += 1;
		if (Keyboard::key(GLFW_KEY_A))
			direction2.x -= 1;
		if (Keyboard::key(GLFW_KEY_W))
			direction2.y -= 1;
		if (Keyboard::key(GLFW_KEY_S))
			direction2.y += 1;

		/*
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

		
		handle->transform.position += direction1 * 10.0f;
		base->transform.position += direction2 * 10.0f;

		leg->solve(length, length, base->transform.position, handle->transform.position);


		p1->transform.position = leg->first;
		p2->transform.position = leg->second;
		p3->transform.position = leg->last;

		// IK Solving(v2 target, float l1, float l2, iterations, step bla)
		
	}
};





