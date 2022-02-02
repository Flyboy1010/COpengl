#ifndef _INPUT_H_
#define _INPUT_H_

#include <GLFW/glfw3.h>

#define NUM_KEYS GLFW_KEY_LAST

void input_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void input_cursor_callback(GLFWwindow *window, double xpos, double ypos);

int input_key_pressed(int key);
int input_key_released(int key);

int input_just_key_pressed(int key);
int input_just_key_released(int key);

void input_mouse(int *x, int *y);

void input_update(void);

#endif // INPUT_H
