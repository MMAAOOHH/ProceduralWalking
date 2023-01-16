#include "Keyboard.h"

bool Keyboard::keys_[GLFW_KEY_LAST] = { false };
bool Keyboard::keys_changed_[GLFW_KEY_LAST] = { false };

void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		if (!keys_[key])
			keys_[key] = true;
	}
	else keys_[key] = false;

	keys_changed_[key] = action != GLFW_REPEAT;
}

bool Keyboard::key(int key)
{
	return keys_[key];
}

bool Keyboard::key_changed(int key)
{
	bool ret =  keys_[key];
	keys_[key] = false;
	return ret;
}

bool Keyboard::key_up(int key)
{
	return !keys_[key] && key_changed(key);
}

bool Keyboard::key_down(int key)
{
	return keys_[key] && key_changed(key);
}
