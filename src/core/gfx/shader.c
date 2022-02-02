#include "core/gfx/shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

static char *shader_parse(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file)
    {
        // first of all get the file size (the trick is to seek to the end and get the position)

        fseek(file, 0, SEEK_END);

        size_t size = ftell(file);

        // return to the begining

        fseek(file, 0, SEEK_SET);

        // allocate space

        char *buffer = malloc(size + 1);

        // read all the file in one take

        fread(buffer, 1, size, file);

        buffer[size] = '\0';

        fclose(file);

        // return it

        return buffer;
    }

    return NULL;
}

static unsigned int shader_compile(char *src, int type)
{
    unsigned int shader_id = glCreateShader(type);

    glShaderSource(shader_id, 1, (const char *const *)&src, NULL);
    glCompileShader(shader_id);

    int compile_status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        char *message = malloc(length);
        glGetShaderInfoLog(shader_id, length, &length, message);

        printf("%s\n", message);

        free(message);

        glDeleteShader(shader_id);

        return 0;
    }

    return shader_id;
}

Shader *shader_load(const char *vertex_shader, const char *fragment_shader)
{
    char *vertex_shader_src = shader_parse(vertex_shader);
    char *fragment_shader_src = shader_parse(fragment_shader);

    unsigned int vertex_shader_id = shader_compile(vertex_shader_src, GL_VERTEX_SHADER);
    unsigned int fragment_shader_id = shader_compile(fragment_shader_src, GL_FRAGMENT_SHADER);

    free(vertex_shader_src);
    free(fragment_shader_src);

    if (vertex_shader_id == 0 || fragment_shader_id == 0)
        return NULL;

    Shader *shader = malloc(sizeof(Shader));

    strncpy(shader->vertex_shader, vertex_shader, 128);
    strncpy(shader->fragment_shader, fragment_shader, 128);
    shader->id = glCreateProgram();

    glAttachShader(shader->id, vertex_shader_id);
    glAttachShader(shader->id, fragment_shader_id);
    glLinkProgram(shader->id);
    glValidateProgram(shader->id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return shader;
}

void shader_destroy(Shader *shader)
{
    if (shader != NULL)
    {
        glDeleteProgram(shader->id);
        free(shader);
    }
}

void shader_bind(const Shader *shader)
{
    if (shader != NULL)
        glUseProgram(shader->id);
    else
        glUseProgram(0);
}

void shader_uniform_1f(const Shader *shader, const char *name, float value)
{
    int uniform_id = glGetUniformLocation(shader->id, name);

    if (uniform_id != -1)
        glUniform1f(uniform_id, value);
    else
        printf("Uniform \"%s\" of type float does not exist!\n", name);
}

void shader_uniform_mat4(const Shader *shader, const char *name, const mat4_t *value)
{
    int uniform_id = glGetUniformLocation(shader->id, name);

    if (uniform_id != -1)
        glUniformMatrix4fv(uniform_id, 1, 1, &value->m[0][0]);
    else
        printf("Uniform \"%s\" of type mat4 does not exist!\n", name);
}

void shader_uniform_v2(const Shader *shader, const char *name, const vec2_t *value)
{
    int uniform_id = glGetUniformLocation(shader->id, name);

    if (uniform_id != -1)
        glUniform2f(uniform_id, value->x, value->y);
    else
        printf("Uniform \"%s\" of type vec2 does not exist!\n", name);
}

void shader_uniform_v3(const Shader *shader, const char *name, const vec3_t *value)
{
    int uniform_id = glGetUniformLocation(shader->id, name);

    if (uniform_id != -1)
        glUniform3f(uniform_id, value->x, value->y, value->z);
    else
        printf("Uniform \"%s\" of type vec3 does not exist!\n", name);
}

void shader_uniform_v4(const Shader *shader, const char *name, const vec4_t *value)
{
    int uniform_id = glGetUniformLocation(shader->id, name);

    if (uniform_id != -1)
        glUniform4f(uniform_id, value->x, value->y, value->z, value->t);
    else
        printf("Uniform \"%s\" of type vec4 does not exist!\n", name);
}

void shader_uniform_1i(const Shader *shader, const char *name, int value)
{
    int uniform_id = glGetUniformLocation(shader->id, name);

    if (uniform_id != -1)
        glUniform1i(uniform_id, value);
    else
        printf("Uniform \"%s\" of type int does not exist!\n", name);
}

void shader_uniform_1iv(const Shader *shader, const char *name, int count, const int *value)
{
    int uniform_id = glGetUniformLocation(shader->id, name);

    if (uniform_id != -1)
        glUniform1iv(uniform_id, count, value);
    else
        printf("Uniform \"%s\" of type int array does not exist!\n", name);
}
