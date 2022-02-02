#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct Window
{
	int width, height;
	int fullscreen, vsync;
	void *native_window_ptr;
} Window;

Window *window_create(const char *title, int width, int height);

void window_destroy(Window *window);

#endif // _WINDOW_H_
