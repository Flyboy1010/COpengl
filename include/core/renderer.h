#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "gfx/texture.h"
#include "gfx/shader.h"
#include "ortho_camera.h"
#include "model.h"

#define MAX_QUADS 10000

void renderer_init(void);
void renderer_destroy(void);

void renderer_clear(float r, float g, float b, float a);
void renderer_viewport(int x, int y, int width, int height);
void renderer_viewport_ar(int window_width, int window_height, float target_aspect_ratio);

void renderer_start_batch(const OrthoCamera *camera);
void renderer_flush(void);

void renderer_texture(Texture *texture, vec2_t *pos, vec2_t *size, vec2_t *src_pos, vec2_t *src_size, vec4_t *color);

void renderer_model(const Model *model, const mat4_t *transform, const mat4_t *projection, const mat4_t *camera_transform, const Shader *shader);

#endif // _RENDERER_H_
