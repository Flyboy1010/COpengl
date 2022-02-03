#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "core/model.h"
#include "core/camera.h"
#include <math.h>

static Model *test_model;
static mat4_t model_transform;
static Shader *shader;
static Camera camera;

static void camera_update(float delta)
{
	static float yaw;
	static float pitch;
	static float old_mouse_x, old_mouse_y;

	const float sensitivity = 0.075f;

	int x, y;
	input_mouse(&x, &y);

	yaw += (x - old_mouse_x) * sensitivity;
	pitch += (y - old_mouse_y) * sensitivity;

	old_mouse_x = x;
    old_mouse_y = y;

    // clamp pitch

    if (pitch < 1.0f) pitch = 1.0f;
    else if (pitch > 179.0f) pitch = 179.0f;
	
	// vectors

	const vec3_t up = { 0, 1, 0 };

	vec3_t forward;
	forward.x = cosf(RADIANS(yaw)) * sinf(RADIANS(pitch));
	forward.y = cosf(RADIANS(pitch));
	forward.z = sinf(RADIANS(yaw)) * sinf(RADIANS(pitch));

	vec3_t right;
	vec3_cross(&forward, &up, &right);
	vec3_normalize(&right, &right);

	// camera motion

	const float velocity = 5.0f;

	if (input_key_pressed(GLFW_KEY_W))
	{
		camera.position.x += forward.x * velocity * delta;
        camera.position.y += forward.y * velocity * delta;
        camera.position.z += forward.z * velocity * delta;
	}
	else if (input_key_pressed(GLFW_KEY_S))
	{
		camera.position.x -= forward.x * velocity * delta;
        camera.position.y -= forward.y * velocity * delta;
        camera.position.z -= forward.z * velocity * delta;
	}
	if (input_key_pressed(GLFW_KEY_D))
	{
		camera.position.x += right.x * velocity * delta;
        camera.position.y += right.y * velocity * delta;
        camera.position.z += right.z * velocity * delta;
	}
	else if (input_key_pressed(GLFW_KEY_A))
	{
		camera.position.x -= right.x * velocity * delta;
        camera.position.y -= right.y * velocity * delta;
        camera.position.z -= right.z * velocity * delta;
	}
	if (input_key_pressed(GLFW_KEY_Q))
	{
		camera.position.x += up.x * velocity * delta;
        camera.position.y += up.y * velocity * delta;
        camera.position.z += up.z * velocity * delta;
	}
	else if (input_key_pressed(GLFW_KEY_E))
	{
		camera.position.x -= up.x * velocity * delta;
        camera.position.y -= up.y * velocity * delta;
        camera.position.z -= up.z * velocity * delta;
	}

	// construct camera transform matrix

	vec3_t target;
	target.x = camera.position.x + forward.x;
	target.y = camera.position.y + forward.y;
	target.z = camera.position.z + forward.z;

	mat4_look_at(&camera.position, &target, &up, &camera.transform);
}

void app_load_config(const char *path, AppConfig *config)
{
	FILE *config_file = fopen(path, "rb");

	if (config_file)
	{
		fread(config, sizeof(AppConfig), 1, config_file);

		fclose(config_file);
	}
}

void app_save_config(const char *path, const AppConfig *config)
{
	FILE *config_file = fopen(path, "wb");

	if (config_file)
	{
		fwrite(config, sizeof(AppConfig), 1, config_file);

		fclose(config_file);
	}
}

int app_init(App *app, const AppConfig *config)
{
	if (!glfwInit())
	{
		printf("glfw failed to init\n");
		return -1;
	}

	// window hints

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);  // the window will stay hidden after creation
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // the window will be resizable
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	glfwWindowHint(GLFW_SAMPLES, 4);

	// create window

	GLFWwindow *window = glfwCreateWindow(config->window_width, config->window_height, "OpenglTemplate", config->window_fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	// Make the OpenGL context current

	glfwMakeContextCurrent(window);

	// Set vsync

	glfwSwapInterval(config->window_vsync);

	// make window visible

	glfwShowWindow(window);

	// init glew

	if (glewInit() != GLEW_OK)
	{
		printf("glew failed to init\n");
		glfwTerminate();
		return -1;
	}

	// input callbacks

	glfwSetKeyCallback(window, input_key_callback);
	glfwSetCursorPosCallback(window, input_cursor_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// display renderer info

	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Version: %s\n", glGetString(GL_VERSION));

	// blending

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// depth testing ...

	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// culling

	glEnable(GL_CULL_FACE);

	// antialiasing

	glEnable(GL_MULTISAMPLE);

	// Application properties

	app->window = window;
	app->config = *config;

	// init renderer

	renderer_init();

	// testing

	test_model = model_load("models/tank", "models/tank/scene.gltf");
	model_dump_debug("model.txt", test_model);

	shader = shader_load("shaders/model.vert", "shaders/model.frag");

	camera_init(&camera, 16.0f / 9.0f, RADIANS(90), 0.1f, 1000.0f);
	mat4_identity(&model_transform);

	return 0;
}

void app_destroy(App *app)
{
	model_destroy(test_model);
	shader_destroy(shader);
	renderer_destroy();
	glfwTerminate();
}

static void app_update(App *app, float delta)
{
	static float time;

	time += delta * 0.1f;

	camera_update(delta);

	vec3_t v = {5.0f, 5.0f, 5.0f};
	mat4_t scale;
	mat4_scaling(&v, &scale);

	mat4_t rotation_x;
	mat4_rotation_x(RADIANS(-90), &rotation_x);

	mat4_t rotation_y;
	mat4_rotation_y(time, &rotation_y);

	mat4_t t1;
	mat4_mult(&rotation_y, &rotation_x, &t1);

	mat4_mult(&t1, &scale, &model_transform);
}

static void app_fixed_update(App *app, float delta)
{
	
}

static void app_render(App *app)
{
	// viewport

	int w, h;
	glfwGetFramebufferSize(app->window, &w, &h);
	renderer_viewport_ar(w, h, 16.0f / 9.0f);

	// clear

	glClearColor(0.15f, 0.15f, 0.15f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render

	renderer_model(test_model, &model_transform, &camera.projection, &camera.transform, &camera.position, shader);

	// swap the window buffers

	glfwSwapBuffers(app->window);
}

static int app_running(App *app)
{
	return (app->running && !glfwWindowShouldClose(app->window));
}

void app_run(App *app)
{
	app->running = 1;

	const double fixed_step = 1.0 / 60.0;

	double time_before_frame = glfwGetTime();
	double time_after_frame = 0;
	double delta = 0;
	double fixed_delta = 0;

	while (app_running(app))
	{
		app_update(app, (float)delta);

		if (fixed_delta >= fixed_step)
		{
			app_fixed_update(app, (float)fixed_step);

			fixed_delta -= fixed_step;
		}

		input_update();

		app_render(app);

		glfwPollEvents();

		// get the delta time

		time_after_frame = glfwGetTime();
		delta = time_after_frame - time_before_frame;
		fixed_delta += delta;
		time_before_frame = time_after_frame;

		// calculate fps

		float fps = 1 / delta;

		char title[50];
		sprintf(title, "OpenglTemplate, %.2f fps", fps);

		glfwSetWindowTitle(app->window, title);
	}
}
