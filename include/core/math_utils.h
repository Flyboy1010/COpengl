#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

#define PI 3.141592653

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(x) ((x) >= 0) ? (x) : -(x)
#define RADIANS(degrees) (degrees * (PI / 180.0f))
#define DEGREES(radians) (radians * (180.0f / PI))
#define LERP(a, b, t) (a + (b - a) * t)

#define VEC2(x, y) (vec2_t) { x, y }
#define VEC3(x, y, z) (vec3_t) { x, y, z }
#define VEC4(x, y, z, w) (vec4_t) { x, y, z, w }

typedef struct vec2_t
{
    float x, y;
} vec2_t;

typedef struct vec3_t
{
    float x, y, z;
} vec3_t;

typedef struct vec4_t
{
    float x, y, z, t;
} vec4_t;

typedef struct mat2_t
{
    float m[2][2];
} mat2_t;

typedef struct mat3_t
{
    float m[3][3];
} mat3_t;

typedef struct mat4_t
{
    float m[4][4];
} mat4_t;

float vec3_length(const vec3_t *v);
float vec3_dot(const vec3_t *v1, const vec3_t *v2);
void vec3_cross(const vec3_t *v1, const vec3_t *v2, vec3_t *out);
void vec3_normalize(const vec3_t *v, vec3_t *out);

void mat4_sub_mat3(const mat4_t *m, int i, int j, mat3_t *out);
float mat3_det(const mat3_t *m);
float mat4_det(const mat4_t *m);

void mat4_identity(mat4_t *out);
void mat4_transpose(const mat4_t *m, mat4_t *out);
void mat4_inverse(const mat4_t *m, mat4_t *out);
void mat4_ortho(float width, float height, mat4_t *out);
void mat4_perspective(float a, float fov, float znear, float zfar, mat4_t *out);
void mat4_look_at(const vec3_t *position, const vec3_t *target, const vec3_t *up, mat4_t *out);
void mat4_translation(const vec3_t *v, mat4_t *out);
void mat4_scaling(const vec3_t *v, mat4_t *out);
void mat4_rotation_x(float radians, mat4_t *out);
void mat4_rotation_y(float radians, mat4_t *out);
void mat4_rotation_z(float radians, mat4_t *out);
void mat4_mult(const mat4_t *m1, const mat4_t *m2, mat4_t *out);

#endif // MATH_UTILS_H