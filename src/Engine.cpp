#include "Engine.h"
#include "Mouse.h"


Engine::Engine(GLuint width, GLuint height)
	: width(width), height(height)
{
};

Engine::~Engine()
{
	/*
	delete &quad_mat;
	delete &circ_mat;
	delete &texture_a;
	delete &texture_b;
	*/
}

void Engine::init()
{
	window = std::make_unique<Window>("TinyEngine", width, height);
	renderer = std::make_unique<SpriteRenderer>();
	camera = std::make_unique<Camera>();

	// Shaders
	auto vs_file_name = "res/Shaders/sprite.vs";
	auto quad_fs_file_name = "res/Shaders/sprite.fs";
	auto circ_fs_file_name = "res/Shaders/circle.fs";


	quad_shader = new Shader();
	quad_shader->load(vs_file_name, quad_fs_file_name);

	circ_shader = new Shader();
	circ_shader->load(vs_file_name, circ_fs_file_name);


	auto projection = camera->get_orthographic_projection();

	quad_shader->use();
	quad_shader->set_mat4("u_projection", projection);

	circ_shader->use();
	circ_shader->set_mat4("u_projection", projection);
	circ_shader->set_vec2f("u_screenResolution", (float)width, (float)height);

	// Texture white
	texture_a = new Texture();
	texture_a->load("res/Images/white.png");
	// Texture png
	texture_b = new Texture();
	texture_b->load("res/Images/beyer.jpg");

	// Materials
	quad_mat = new Material(texture_a, quad_shader, 0);
	circ_mat = new Material(texture_a, circ_shader, 0);
	circ_mat2 = new Material(texture_a, circ_shader, 0);
}


void Engine::handle_input()
{
	window->process_events();
}

double old_time = 0;
void Engine::update()
{
	// delta time
	double time = glfwGetTime();
	delta_time = static_cast<GLfloat>(time - old_time);


	// update camera
	// Zoom
	camera->zoom = lerp(camera->zoom, camera->zoom + static_cast<GLfloat>(Mouse::get_scroll_dy()), camera->zoom_sensitivity * delta_time);
	//  Pan
	if (Mouse::button(1))
		camera->position += glm::vec2(Mouse::get_mouse_dx(), Mouse::get_mouse_dy());

	// Update objects
	for (const auto& entry : objects)
		entry->update(delta_time);

	old_time = time;
}

void Engine::draw_circle(glm::vec2 pos, GLfloat size)
{
	auto circ = std::make_shared<struct Drawable>();
	circ->material = circ_mat;
	circ->position = pos;
	circ->size *= size;

	renderer->draw(*circ, camera->transform_view());
}

void Engine::render()
{
	window->clear();

	for (auto& game_object : objects)
		game_object->draw(*renderer, camera->transform_view());

	window->update();
}

std::shared_ptr<GameObject> Engine::add_game_object()
{
	auto go = std::make_shared<GameObject>();
	objects.push_back(go);
	go->start();

	return go;
}

std::shared_ptr<GameObject> Engine::add_circle_object(GLfloat size)
{
	auto go = std::make_shared<GameObject>();
	objects.push_back(go);

	auto circ = std::make_shared<struct Drawable>();
	circ->material = circ_mat2;
	circ->size *= size;

	go->drawable = *circ;
	go->start();

	return go;
}
