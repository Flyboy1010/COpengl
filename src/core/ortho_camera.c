#include "core/ortho_camera.h"

void ortho_camera_init(OrthoCamera *camera, float width, float height)
{
    camera->position = VEC2(0, 0);
    mat4_ortho(width, height, &camera->projection);
}
