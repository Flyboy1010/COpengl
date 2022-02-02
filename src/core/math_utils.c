#include "core/math_utils.h"
#include <math.h>
#include <string.h>

float vec3_length(const vec3_t *v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

float vec3_dot(const vec3_t *v1, const vec3_t *v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

void vec3_cross(const vec3_t *v1, const vec3_t *v2, vec3_t *out)
{
    out->x = v1->y * v2->z - v1->z * v2->y;
    out->y = v1->z * v2->x - v1->x * v2->z;
    out->z = v1->x * v2->y - v1->y * v2->x;
}

void vec3_normalize(const vec3_t *v, vec3_t *out)
{
    float inv_length = 1 / sqrt(v->x * v->x + v->y * v->y + v->z * v->z);

    out->x = v->x * inv_length;
    out->y = v->y * inv_length;
    out->z = v->z * inv_length;
}

void mat4_identity(mat4_t *out)
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            out->m[j][i] = (i == j) ? 1 : 0;
        }
    }
}

void mat4_transpose(const mat4_t *m, mat4_t *out)
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            out->m[j][i] = m->m[i][j];
        }
    }
}

void mat4_sub_mat3(const mat4_t *m, int i, int j, mat3_t *out)
{
    int sub_i = 0;
    int sub_j = 0;

    for (int m_j = 0; m_j < 4; m_j++)
    {
        for (int m_i = 0; m_i < 4; m_i++)
        {
            if (m_i != i && m_j != j)
            {
                out->m[sub_j][sub_i] = m->m[m_j][m_i];

                if (sub_i < 2)
                {
                    sub_i++;
                }
                else
                {
                    sub_i = 0;
                    sub_j++;
                }
            }
        }   
    }
}

float mat3_det(const mat3_t *m)
{
    return    (m->m[0][0] * m->m[1][1] * m->m[2][2]) 
            + (m->m[0][1] * m->m[1][2] * m->m[2][0])
            + (m->m[1][0] * m->m[2][1] * m->m[0][2])
            - (m->m[2][0] * m->m[1][1] * m->m[0][2])
            - (m->m[2][1] * m->m[1][2] * m->m[0][0])
            - (m->m[1][0] * m->m[0][1] * m->m[2][2]);
}

float mat4_det(const mat4_t *m)
{
    // calculate sub_3x3_matrix determinants

    float sub_det[4];

    for (int j = 0; j < 4; j++)
    {
        mat3_t sub_mat3_t;
        mat4_sub_mat3(m, 0, j, &sub_mat3_t);

        sub_det[j] = mat3_det(&sub_mat3_t);
    }

    // calculate determinant

    float det = m->m[0][0] * sub_det[0] - m->m[0][1] * sub_det[1] + m->m[0][2] * sub_det[2] - m->m[0][3] * sub_det[3];

    return det;
}

void mat4_inverse(const mat4_t *m, mat4_t *out)
{
    // get the 3x3 sub matrices and its det

    mat3_t sub_mat[4][4];
    float sub_det[4][4];

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            mat4_sub_mat3(m, i, j, &sub_mat[j][i]);
            sub_det[j][i] = mat3_det(&sub_mat[j][i]);
        }
    }

    // calculate the determinant of the 4x4 mat

    float det = m->m[0][0] * sub_det[0][0] - m->m[0][1] * sub_det[0][1] + m->m[0][2] * sub_det[0][2] - m->m[0][3] * sub_det[0][3];

    // check if the determinant is 0

    if (det != 0)
    {
        // calculate the inverse matrix

        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                if ((i + j) % 2 == 0)
                    out->m[j][i] = sub_det[i][j];
                else
                    out->m[j][i] = -sub_det[i][j];

                out->m[j][i] /= det;
            }
        }
    }
}

void mat4_ortho(float width, float height, mat4_t *out)
{
    out->m[0][0] = 2 / width;
    out->m[0][1] = 0;
    out->m[0][2] = 0;
    out->m[0][3] = -1;

    out->m[1][0] = 0;
    out->m[1][1] = -2 / height;
    out->m[1][2] = 0;
    out->m[1][3] = 1;

    out->m[2][0] = 0;
    out->m[2][1] = 0;
    out->m[2][2] = 1;
    out->m[2][3] = 0;

    out->m[3][0] = 0;
    out->m[3][1] = 0;
    out->m[3][2] = 0;
    out->m[3][3] = 1;
}

void mat4_perspective(float a, float fov, float znear, float zfar, mat4_t *out)
{
    memset(out, 0, sizeof(mat4_t));

    float inv_tan_fov = 1 / (float)tan(fov / 2.0f);

    out->m[0][0] = (1 / a) * inv_tan_fov;
    out->m[1][1] = inv_tan_fov;
    out->m[2][2] = -(zfar + znear) / (zfar - znear);
    out->m[2][3] = -(2 * zfar * znear) / (zfar - znear);
    out->m[3][2] = -1;
}

void mat4_look_at(const vec3_t *position, const vec3_t *target, const vec3_t *up, mat4_t *out)
{
    // up vector

    vec3_t normalized_up;
    vec3_normalize(up, &normalized_up);

    // direction vector (actually reversed)

    vec3_t direction;
    direction.x = (position->x - target->x);
    direction.y = (position->y - target->y);
    direction.z = (position->z - target->z);

    vec3_t normalized_direction;
    vec3_normalize(&direction, &normalized_direction);

    // right vector

    vec3_t right;
    vec3_cross(&normalized_up, &normalized_direction, &right);

    vec3_t normalized_right;
    vec3_normalize(&right, &normalized_right);

    // camera up

    vec3_t camera_up;
    vec3_cross(&normalized_direction, &normalized_right, &camera_up);

    vec3_t normalized_camera_up;
    vec3_normalize(&camera_up, &normalized_camera_up);

    // construct "rotation matrix"

    mat4_t rotation;

    rotation.m[0][0] = normalized_right.x;
    rotation.m[0][1] = normalized_right.y;
    rotation.m[0][2] = normalized_right.z;
    rotation.m[0][3] = 0;
    rotation.m[1][0] = normalized_camera_up.x;
    rotation.m[1][1] = normalized_camera_up.y;
    rotation.m[1][2] = normalized_camera_up.z;
    rotation.m[1][3] = 0;
    rotation.m[2][0] = normalized_direction.x;
    rotation.m[2][1] = normalized_direction.y;
    rotation.m[2][2] = normalized_direction.z;
    rotation.m[2][3] = 0;
    rotation.m[3][0] = 0;
    rotation.m[3][1] = 0;
    rotation.m[3][2] = 0;
    rotation.m[3][3] = 1;

    // translation matrix
    
    mat4_t translation;
    mat4_identity(&translation);

    translation.m[0][3] = -position->x;
    translation.m[1][3] = -position->y;
    translation.m[2][3] = -position->z;

    // look at is the multiplication of the rotation and translation matrix

    mat4_mult(&rotation, &translation, out);
}

void mat4_translation(const vec3_t *v, mat4_t *out)
{
    mat4_identity(out);

    out->m[0][3] = v->x;
    out->m[1][3] = v->y;
    out->m[2][3] = v->z;
}

void mat4_scaling(const vec3_t *v, mat4_t *out)
{
    memset(out, 0, sizeof(mat4_t));

    out->m[0][0] = v->x;
    out->m[1][1] = v->y;
    out->m[2][2] = v->z;
    out->m[3][3] = 1;
}

void mat4_rotation_x(float radians, mat4_t *out)
{
    memset(out, 0, sizeof(mat4_t));

    float cos_angle = cos(radians);
    float sin_angle = sin(radians);

    out->m[0][0] = 1;
    out->m[1][1] = cos_angle;
    out->m[1][2] = -sin_angle;
    out->m[2][1] = sin_angle;
    out->m[2][2] = cos_angle;
    out->m[3][3] = 1;
}

void mat4_rotation_y(float radians, mat4_t *out)
{
    memset(out, 0, sizeof(mat4_t));

    float cos_angle = cos(radians);
    float sin_angle = sin(radians);

    out->m[0][0] = cos_angle;
    out->m[0][2] = sin_angle;
    out->m[1][1] = 1;
    out->m[2][0] = -sin_angle;
    out->m[2][2] = cos_angle;
    out->m[3][3] = 1;
}

void mat4_rotation_z(float radians, mat4_t *out)
{
    memset(out, 0, sizeof(mat4_t));

    float cos_angle = cos(radians);
    float sin_angle = sin(radians);

    out->m[0][0] = cos_angle;
    out->m[0][1] = -sin_angle;
    out->m[1][0] = sin_angle;
    out->m[1][1] = cos_angle;
    out->m[2][2] = 1;
    out->m[3][3] = 1;
}

void mat4_mult(const mat4_t *m1, const mat4_t *m2, mat4_t *out)
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            out->m[j][i] = m1->m[j][0] * m2->m[0][i] + m1->m[j][1] * m2->m[1][i] + m1->m[j][2] * m2->m[2][i] + m1->m[j][3] * m2->m[3][i];
        }
    }
}
