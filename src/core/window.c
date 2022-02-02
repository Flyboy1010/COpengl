#include "core/window.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int num_windows = 0;

Window *window_create(const char *title, int width, int height)
{
	if (num_windows == 0)
	{
		if (!glfwInit())
		{
			printf("glfw failed to init\n");
			return NULL;
		}
	}

	// increment window number
	
	num_windows++;

	// window hints

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);  // the window will stay hidden after creation
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // the window will be resizable
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window

	GLFWwindow *glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!glfw_window)
	{
		printf("Failed to create window %s\n", title);
		return NULL;
	}

	// Make the OpenGL context current

	glfwMakeContextCurrent(glfw_window);

	// Set vsync

	glfwSwapInterval(1);

	// make window visible

	glfwShowWindow(glfw_window);

	// init glew

	if (glewInit() != GLEW_OK)
	{
		printf("glew failed to init\n");
		return NULL;
	}

	// display renderer info

	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Version: %s\n", glGetString(GL_VERSION));

	// allocate memory for the window
	
	Window *window = malloc(sizeof(Window));

	window->native_window_ptr = glfw_window;
	window->width = width;
	window->height = height;
	window->fullscreen = 0;
	window->vsync = 1;

	return window;
}

void window_destroy(Window *window)
{
	glfwDestroyWindow(window->native_window_ptr);
	free(window);
}
