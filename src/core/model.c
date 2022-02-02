#include "core/model.h"
#include <stdio.h>
#include <stdlib.h>
#include "core/math_utils.h"
#include <GL/glew.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

typedef struct Vertex
{
    vec3_t position;
    vec3_t normal;
    vec2_t texture_uv;
} Vertex;

static void generate_mesh(const Vertex *vertex_data, int vertices_count, const unsigned int *index_data, int indices_count, const material_t *material, Mesh *output_mesh)
{
    // mesh specs

    output_mesh->vertices_count = vertices_count;
    output_mesh->indices_count = indices_count;
    output_mesh->material = *material;

    // vertex array object

    glGenVertexArrays(1, &output_mesh->vao);
    glBindVertexArray(output_mesh->vao);

    // vertex buffer object

    glGenBuffers(1, &output_mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, output_mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(Vertex), vertex_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, texture_uv));

    // index buffer object

    glGenBuffers(1, &output_mesh->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output_mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned int), index_data, GL_STATIC_DRAW);
}

static Texture *get_material_texture(Model *model, const struct aiMaterial *ai_material, enum aiTextureType texture_type)
{
    if (aiGetMaterialTextureCount(ai_material, texture_type) > 0)
    {
        // local path is relative to the model file

        struct aiString ai_relative_path;
        aiGetMaterialTexture(ai_material, texture_type, 0, &ai_relative_path, NULL, NULL, NULL, NULL, NULL, NULL);

        // get the full path

        char path[128];
        snprintf(path, 128, "%s/%s", model->dir, ai_relative_path.data);

        // check if is the texture already loaded comparing the path

        for (int i = 0; i < model->textures->num_elements; i++)
        {
            Texture *texture = vector_element(Texture *, model->textures, i);

            // if found return the texture pointer

            if (strcmp(path, texture->path) == 0)
                return texture;
        }

        // if is not already loaded then load it and store in the textures vector

        Texture *texture = texture_load(path, 0);
        vector_push(model->textures, &texture);

        return texture;
    }

    return NULL;
}

Model *model_load(const char *dir, const char *path)
{
    const struct aiScene *ai_scene = aiImportFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

    if (!ai_scene)
    {
        printf("Couldn't load %s\n", path);
        return NULL;
    }

    // create model

    Model *model = malloc(sizeof(Model));

    // model properties

    model->path = path;
    model->dir = dir;
    model->meshes_count = ai_scene->mNumMeshes;
    model->meshes = malloc(ai_scene->mNumMeshes * sizeof(Mesh));
    model->textures = vector_create(sizeof(Texture *));

    // for each mesh

    for (int i = 0; i < ai_scene->mNumMeshes; i++)
    {
        const struct aiMesh *ai_mesh = ai_scene->mMeshes[i];

        // get vertex data

        Vertex *vertex_data = malloc(ai_mesh->mNumVertices * sizeof(Vertex));

        for (int j = 0; j < ai_mesh->mNumVertices; j++)
        {
            Vertex *v = &vertex_data[j];

            // vertices position

            v->position.x = ai_mesh->mVertices[j].x;
            v->position.y = ai_mesh->mVertices[j].y;
            v->position.z = ai_mesh->mVertices[j].z;

            // vertices normal

            v->normal.x = ai_mesh->mNormals[j].x;
            v->normal.y = ai_mesh->mNormals[j].y;
            v->normal.z = ai_mesh->mNormals[j].z;

            // vertices texture uv

             if (ai_mesh->mTextureCoords[0] != NULL)
            {
                v->texture_uv.x = ai_mesh->mTextureCoords[0][j].x;
                v->texture_uv.y = ai_mesh->mTextureCoords[0][j].y;
            }
        }

        // get index data

        unsigned int *index_data = malloc(ai_mesh->mNumFaces * 3 * sizeof(unsigned int));

        for (int j = 0; j < ai_mesh->mNumFaces; j++)
        {
            const struct aiFace *ai_face = &ai_mesh->mFaces[j];

            // 3 indices because the faces are triangles

            index_data[3 * j] = ai_face->mIndices[0];
            index_data[3 * j + 1] = ai_face->mIndices[1];
            index_data[3 * j + 2] = ai_face->mIndices[2];
        }

        // get material data

        material_t material = {0};

        if (ai_mesh->mMaterialIndex >= 0)
        {
            const struct aiMaterial *ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];

            material.texture_diffuse = get_material_texture(model, ai_material, aiTextureType_DIFFUSE);
            // aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &material.color_diffuse);
        }

        // generate mesh

        generate_mesh(vertex_data, ai_mesh->mNumVertices, index_data, ai_mesh->mNumFaces * 3, &material, &model->meshes[i]);

        // free data

        free(vertex_data);
        free(index_data);
    }

    // clean

    aiReleaseImport(ai_scene);

    return model;
}

void model_destroy(Model *model)
{
    for (int i = 0; i < model->meshes_count; i++)
    {
        const Mesh *mesh = &model->meshes[i];

        glDeleteVertexArrays(1, &mesh->vao);
        glDeleteBuffers(1, &mesh->vbo);
        glDeleteBuffers(1, &mesh->ibo);
    }

    for (int i = 0; i < model->textures->num_elements; i++)
    {
        texture_destroy(vector_element(Texture *, model->textures, i));
    }

    vector_destroy(model->textures);
    free(model->meshes);
    free(model);
}

void model_dump_debug(const char *path, const Model *model)
{
    FILE *file = fopen(path, "w");

    if (file != NULL)
    {
        fprintf(file, "Model path: %s\n", model->path);
        fprintf(file, "Meshes count: %d\n\n", model->meshes_count);

        int total_vertices = 0;
        int total_indices = 0;
        int total_triangles = 0;

        for (int i = 0; i < model->meshes_count; i++)
        {
            const Mesh *mesh = &model->meshes[i];
            const material_t *material = &mesh->material;

            total_vertices += mesh->vertices_count;
            total_indices += mesh->indices_count;
            total_triangles += mesh->indices_count / 3;

            fprintf(file, "Mesh %d:\n", i);
            fprintf(file, "\tVertices count: %d\n", mesh->vertices_count);
            fprintf(file, "\tIndices count: %d\n", mesh->indices_count);

            if (material->texture_diffuse != NULL)
                fprintf(file, "\tTexture diffuse path: %s\n", material->texture_diffuse->path);
        }

        fprintf(file, "Model vertices: %d\n", total_vertices);
        fprintf(file, "Model indices: %d\n", total_indices);
        fprintf(file, "Model triangles: %d\n", total_triangles);

        // close the file

        fclose(file);
    }
}