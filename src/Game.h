#include <memory>
#include <glad/glad.h>

#include "Engine.h"
#include "Keyboard.h"

class IKChain
{

public:
	glm::vec2 v1, v2, v3;

private:
	 // v1 is the root, v3 is the end effector
	GLfloat lenght;
	GLfloat distance;
	GLfloat rotation_angle;
	GLfloat rotation_angle2;


public:
	IKChain(float lenght, float distance)
		: lenght(lenght), distance(distance), rotation_angle(0.0f), rotation_angle2(0.0f)
	{
		v1 = { 0.0f, 0.0f };
		v2 = { 0.0f, lenght };
		v3 = { distance, lenght };
	}

	void solve(glm::vec2 target, int maxIterations, float threshold, float stepSize)
	{
		for (int i = 0; i < maxIterations; i++) {
			glm::vec2 error = calculate_error(target);
			if (glm::length(error) < threshold) {
				break;
			}

			GLfloat a1 = calculate_angle_first();
			GLfloat a2 = calculate_angle_second();
			update_angles(a1, a2, stepSize);
			update_joint_positions(a1);
		}
	}

	glm::vec2 calculate_error(glm::vec2 target)
	{
		return target - v3;
	}

	GLfloat calculate_angle_first()
	{
		return acos(glm::length(v3 - v2) / distance);
	}

	GLfloat calculate_angle_second()
	{
		// TODO: remove atan2 and use dot with normalized vectors instead.
		return atan2(v2.y - v1.y, v2.x - v1.x);
	}

	void update_angles(GLfloat angle, GLfloat angle2, GLfloat stepSize)
	{
		rotation_angle += angle * stepSize;
		rotation_angle2 += angle2 * stepSize;
	}

	void update_joint_positions(GLfloat angle)
	{
		// "Elbow"
		// x = cosine of the -angle + cosine of the radian * length, y = sine of the angle + radian * length
		v2 = { (glm::cos(-angle + rotation_angle)) * lenght, glm::sin(-angle + rotation_angle) * lenght };

		// End Effector, "ankle"
		// x = cosine of the radian * distance, y = sine of the radian * distance
		v3 = { glm::cos(rotation_angle) * distance, glm::sin(rotation_angle) * distance };
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

	std::shared_ptr<GameObject> p1, p2, p3, handle;
	glm::vec2 v1 = { 0,0 };
	glm::vec2 v2 = { 0,0 };
	glm::vec2 v3 = { 0,0 };

	// IK Testing
	std::shared_ptr<IKChain> leg;
	GLfloat lenght = 200.0f;
	GLfloat distance = 300.0f;


	void start() override
	{
		p1 = engine->add_game_object();
		p2 = engine->add_game_object();
		p3 = engine->add_game_object();
		handle = engine->add_game_object();
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

		leg = std::make_shared<IKChain>(lenght, distance);

	}


	 		
	GLfloat rotation_angle = glm::half_pi<float>();

	GLfloat elapsed = 0;

	void update(GLfloat dt) override
	{
		glm::vec2 direction = { 0,0 };

		if (Keyboard::key(GLFW_KEY_RIGHT))
			rotation_angle += 0.01f;
		if (Keyboard::key(GLFW_KEY_LEFT))
			rotation_angle -= 0.01f;
		if (Keyboard::key(GLFW_KEY_UP))
			distance += 10;
		if (Keyboard::key(GLFW_KEY_DOWN))
			distance -= 10;

		// FK testing

		glm::clamp(rotation_angle, 0.0f, glm::two_pi<GLfloat>());

		// inverse cosine, distance / lenght * 2
		GLfloat angle = glm::acos(distance / (lenght * 2));

		// Calculate "Elbow"
		// ----------------------
		// x = angle + cosine of the radian * length, y = sine of the angle + radian * length
		v2 = { (glm::cos(-angle + rotation_angle)) * lenght, glm::sin(-angle + rotation_angle) * lenght };

		// Calculate End Effector, "ankle"
		// ----------------------
		// radian 90 degrees over pi
		// x = cosine of the radian * distance, y = sine of the radian * distance
		v3 = { glm::cos(rotation_angle) * distance, glm::sin(rotation_angle) * distance };


		p1->transform.position = v1;
		p2->transform.position = v2;
		p3->transform.position = v3;

	}
};





