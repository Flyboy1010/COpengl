#ifndef _MODEL_H_
#define _MODEL_H_

#include "gfx/texture.h"
#include "math_utils.h"
#include "vector.h"

typedef struct material_t
{
    Texture *texture_diffuse;
} material_t;

typedef struct Mesh
{
    unsigned int vao, vbo, ibo;
    int vertices_count, indices_count;
    material_t material;
} Mesh;

typedef struct Model
{
    Mesh *meshes;
    int meshes_count;
    Vector *textures;
    const char *path;
    const char *dir;
} Model;

Model *model_load(const char *dir, const char *path);

void model_destroy(Model *model);

void model_dump_debug(const char *path, const Model *model);

#endif // _MODEL_H_