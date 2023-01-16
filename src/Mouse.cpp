#include "Mouse.h"


double Mouse::mouse_x_;
double Mouse::mouse_y_;
double Mouse::mouse_x;
double Mouse::mouse_y;


double Mouse::last_x_;
double Mouse::last_y_;

double Mouse::mouse_dx_;
double Mouse::mouse_dy_;

double Mouse::scroll_dx_;
double Mouse::scroll_dy_;

bool Mouse::first_mouse;

bool Mouse::buttons_[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::buttons_changed_[GLFW_MOUSE_BUTTON_LAST] = { 0 };


void Mouse::cursor_pos_callback(GLFWwindow* window, double pos_x, double pos_y)
{
	mouse_x_ = pos_x;
	mouse_y_ = pos_y;
	mouse_x = pos_x;
	mouse_y = pos_y;

	if(first_mouse)
	{
		last_x_ = mouse_x_;
		last_y_ = mouse_y_;
		first_mouse = false;
	}

	mouse_dx_ = last_x_ - pos_x;
	mouse_dy_ = last_y_ - pos_y;
	last_x_ = pos_x;
	last_y_ = pos_y;
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		if (!buttons_[button])
		{
			buttons_[button] = true;
		}
	}
	else
	{
		buttons_[button] = false;
	}

	buttons_changed_[button] = action != GLFW_REPEAT;
}

void Mouse::mouse_wheel_callback(GLFWwindow* window, double dx, double dy)
{
	scroll_dx_ = dx;
	scroll_dy_ = dy;
}

double Mouse::get_mouse_x()
{
	return mouse_x_;
}

double Mouse::get_mouse_y()
{
	return mouse_y_;
}

double Mouse::get_mouse_dx()
{
	double dx = mouse_dx_;
	mouse_dx_ = 0;
	return dx;
}

double Mouse::get_mouse_dy()
{
	double dy = mouse_dy_;
	mouse_dy_ = 0;
	return dy;
}

double Mouse::get_scroll_dx()
{
	double dx = scroll_dx_;
	scroll_dx_ = 0;
	return dx;
}

double Mouse::get_scroll_dy()
{
	double dy = scroll_dy_;
	scroll_dy_ = 0;
	return dy;
}

bool Mouse::button(int button)
{
	return buttons_[button];
}

bool Mouse::button_changed(int button)
{
	bool ret = buttons_changed_[button];
	buttons_changed_[button] = false;
	return ret;
}

bool Mouse::button_up(int button)
{
	return !buttons_[button] && button_changed(button);
}

bool Mouse::button_down(int button)
{
	return buttons_[button] && button_changed(button);
}
