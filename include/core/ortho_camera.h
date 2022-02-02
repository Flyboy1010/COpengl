#ifndef _ORTHO_CAMERA_H_
#define _ORTHO_CAMERA_H_

#include "math_utils.h"

typedef struct OrthoCamera
{
    vec2_t position;
    mat4_t projection;
} OrthoCamera;

void ortho_camera_init(OrthoCamera *camera, float width, float height);

#endif // ORTHO_CAMERA_H