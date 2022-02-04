#include "core/camera.h"
#include "core/input.h"
#include <math.h>
#include <stdio.h>

void camera_init(Camera *camera, float a, float fov, float znear, float zfar)
{
    camera->position = VEC3(0, 0, 0);
    mat4_perspective(a, fov, znear, zfar, &camera->projection);
    mat4_identity(&camera->transform);
    camera->yaw = 0;
    camera->pitch = 90;
}

void camera_free_move(Camera *camera, float delta)
{
	static float old_mouse_x, old_mouse_y;

	const float sensitivity = 0.075f;

	int x, y;
	input_mouse(&x, &y);

	camera->yaw += (x - old_mouse_x) * sensitivity;
	camera->pitch += (y - old_mouse_y) * sensitivity;

	old_mouse_x = x;
    old_mouse_y = y;

    // clamp pitch

    if (camera->pitch < 1.0f) camera->pitch = 1.0f;
    else if (camera->pitch > 179.0f) camera->pitch = 179.0f;
	
	// vectors

	const vec3_t up = { 0, 1, 0 };

	vec3_t forward;
	forward.x = cosf(RADIANS(camera->yaw)) * sinf(RADIANS(camera->pitch));
	forward.y = cosf(RADIANS(camera->pitch));
	forward.z = sinf(RADIANS(camera->yaw)) * sinf(RADIANS(camera->pitch));

	vec3_t right;
	vec3_cross(&forward, &up, &right);
	vec3_normalize(&right, &right);

	// camera motion

	const float velocity = 5.0f;

	if (input_key_pressed(GLFW_KEY_W))
	{
		camera->position.x += forward.x * velocity * delta;
        camera->position.y += forward.y * velocity * delta;
        camera->position.z += forward.z * velocity * delta;
	}
	else if (input_key_pressed(GLFW_KEY_S))
	{
		camera->position.x -= forward.x * velocity * delta;
        camera->position.y -= forward.y * velocity * delta;
        camera->position.z -= forward.z * velocity * delta;
	}
	if (input_key_pressed(GLFW_KEY_D))
	{
		camera->position.x += right.x * velocity * delta;
        camera->position.y += right.y * velocity * delta;
        camera->position.z += right.z * velocity * delta;
	}
	else if (input_key_pressed(GLFW_KEY_A))
	{
		camera->position.x -= right.x * velocity * delta;
        camera->position.y -= right.y * velocity * delta;
        camera->position.z -= right.z * velocity * delta;
	}
	if (input_key_pressed(GLFW_KEY_Q))
	{
		camera->position.x += up.x * velocity * delta;
        camera->position.y += up.y * velocity * delta;
        camera->position.z += up.z * velocity * delta;
	}
	else if (input_key_pressed(GLFW_KEY_E))
	{
		camera->position.x -= up.x * velocity * delta;
        camera->position.y -= up.y * velocity * delta;
        camera->position.z -= up.z * velocity * delta;
	}

	// construct camera transform matrix

	vec3_t target;
	target.x = camera->position.x + forward.x;
	target.y = camera->position.y + forward.y;
	target.z = camera->position.z + forward.z;

	mat4_look_at(&camera->position, &target, &up, &camera->transform);
}