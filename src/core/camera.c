#include "core/camera.h"

void camera_init(Camera *camera, float a, float fov, float znear, float zfar)
{
    camera->position = VEC3(0, 0, 0);
    mat4_perspective(a, fov, znear, zfar, &camera->projection);
    mat4_identity(&camera->transform);
}