#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class Keyboard
{
public:
	//Keystate callback
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool key(int key);
	static bool key_changed(int key);
	static bool key_up(int key);
	static bool key_down(int key);

private:
	static bool keys_[];
	static bool keys_changed_[];
};