#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

class Mouse
{
public:
	static void cursor_pos_callback(GLFWwindow* window, double pos_x, double pos_y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_wheel_callback(GLFWwindow* window, double dx, double dy);

	static double get_mouse_x();
	static double get_mouse_y();
	static double get_mouse_dx();
	static double get_mouse_dy();
	static double get_scroll_dx();
	static double get_scroll_dy();

	static bool button(int button);
	static bool button_changed(int button);
	static bool button_up(int button);
	static bool button_down(int button);

	static double mouse_x;
	static double mouse_y;

private:
	static double mouse_x_;
	static double mouse_y_;

	static double last_x_;
	static double last_y_;

	static double mouse_dx_;
	static double mouse_dy_;

	static double scroll_dx_;
	static double scroll_dy_;

	static bool first_mouse;

	static bool buttons_[];
	static bool buttons_changed_[];
};
