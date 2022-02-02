#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "math_utils.h"

typedef struct Camera
{
    mat4_t projection;
    mat4_t transform;
    vec3_t position;
} Camera;

void camera_init(Camera *camera, float a, float fov, float znear, float zfar);

#endif // _CAMERA_H_