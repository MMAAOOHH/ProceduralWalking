#include <memory>
#include <glad/glad.h>

#include "Engine.h"
#include "Keyboard.h"

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

	std::shared_ptr<GameObject> p1, p2, p3, line;
	glm::vec2 v1, v2, v3 = { 0,0 };



	void start() override
	{
		p1 = engine->add_game_object();
		p2 = engine->add_game_object();
		p3 = engine->add_game_object();
		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat;
			circ->material->color = { 1,0.5,0 };
			p1->drawable = *circ;
		}
		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat;
			circ->material->color = { 1,0.5,0 };
			p2->drawable = *circ;
		}
		{
			auto circ = std::make_shared<struct Drawable>();
			circ->material = engine->circ_mat;
			circ->material->color = { 1,0.5,0 };
			p3->drawable = *circ;
		}

	}

	float lenght = 150.0f;
	float distance = 200.0f;
	float angle = glm::acos(distance / (lenght * 2)); 		// inverse cosine, distance / lenght * 2
	float arm_angle = 1;

	void update(GLfloat dt) override
	{
		glm::vec2 direction = { 0,0 };

		if (Keyboard::key(GLFW_KEY_RIGHT))
			arm_angle += 0.01f;
		if (Keyboard::key(GLFW_KEY_LEFT))
			arm_angle -= 0.01f;
		if (Keyboard::key(GLFW_KEY_UP))
			distance += 10;
		if (Keyboard::key(GLFW_KEY_DOWN))
			distance -= 10;


		glm::clamp(arm_angle, 0.0f, glm::two_pi<float>());
		float angle = glm::acos(distance / (lenght * 2));
		


		// Calculate "Elbow"
		// ----------------------
		// x = angle + cosine of the radian * length, y = sine of the angle + radian * length
		v2 = { (glm::cos(-angle + arm_angle)) * lenght, glm::sin(-angle + arm_angle) * lenght };

		// Calculate End Effector
		// ----------------------
		// radian 90 degrees over pi
		// x = cosine of the radian * distance, y = sine of the radian * distance
		v3 = { glm::cos(arm_angle) * distance, glm::sin(arm_angle) * distance };


		p1->transform.position += direction;
		p2->transform.position = v2;
		p3->transform.position = v3;

		std::cout << std::to_string(angle) << std::endl;

	}
};


