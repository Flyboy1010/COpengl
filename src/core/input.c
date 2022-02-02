#include "core/input.h"
#include <string.h>
#include <stdint.h>

static uint8_t keys_old[NUM_KEYS];
static uint8_t keys[NUM_KEYS];

static int mouse_x, mouse_y;

void input_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key >= 0 && key < NUM_KEYS)
	{
		keys[key] = (action != GLFW_RELEASE);
	}
}

void input_cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
	mouse_x = (int)xpos;
	mouse_y = (int)ypos;
}

int input_key_pressed(int key)
{
	return keys[key];
}

int input_key_released(int key)
{
	return !keys[key];
}

int input_just_key_pressed(int key)
{
	return (keys[key] && !keys_old[key]);
}

int input_just_key_released(int key)
{
	return (!keys[key] && keys_old[key]);
}

void input_mouse(int *x, int *y)
{
	*x = mouse_x;
	*y = mouse_y;
}

void input_update(void)
{
	memcpy(keys_old, keys, NUM_KEYS * sizeof(uint8_t));
}
