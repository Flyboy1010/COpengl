#ifndef _SHADER_H_
#define _SHADER_H_

#include "core/math_utils.h"

typedef struct Shader
{
    unsigned int id;
    char vertex_shader[128];
    char fragment_shader[128];
} Shader;

Shader *shader_load(const char *vertex_shader, const char *fragment_shader);
void shader_destroy(Shader *shader);

void shader_bind(const Shader *shader);

void shader_uniform_1f(const Shader *shader, const char *name, float value);
void shader_uniform_mat4(const Shader *shader, const char *name, const mat4_t *value);
void shader_uniform_v2(const Shader *shader, const char *name, const vec2_t *value);
void shader_uniform_v3(const Shader *shader, const char *name, const vec3_t *value);
void shader_uniform_v4(const Shader *shader, const char *name, const vec4_t *value);
void shader_uniform_1i(const Shader *shader, const char *name, int value);
void shader_uniform_1iv(const Shader *shader, const char *name, int count, const int *value);

#endif // SHADER_H
