#include "core/gfx/texture.h"
#include <GL/glew.h>
#include "stb_image/stb_image.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Texture *texture_create(int width, int height)
{
    Texture *texture = malloc(sizeof(Texture));

    texture->width = width;
    texture->height = height;
    texture->data = NULL;
    strcpy(texture->path, "");

    // opengl create Texture

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // opengl Texture parameters

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // set data

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    return texture;
}

Texture *texture_load(const char *path, int keep_data)
{
    // load image

    stbi_set_flip_vertically_on_load(1);

    int width, height, bpp;
    unsigned char *data = stbi_load(path, &width, &height, &bpp, 4);

    if (data == NULL)
    {
        printf("Couldn'texture load %s\n", path);
        return NULL;
    }

    // allocate memory

    Texture *texture = malloc(sizeof(Texture));

    texture->width = width;
    texture->height = height;
    texture->bpp = bpp;
    strncpy(texture->path, path, 128);

    // opengl create Texture

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // opengl Texture parameters

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // set data

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // generate mipmaps

    glGenerateMipmap(GL_TEXTURE_2D);

    // free data or not

    if (keep_data)
        texture->data = data;
    else
    {
        stbi_image_free(data);
        texture->data = NULL;
    }

    return texture;
}

void texture_destroy(Texture *texture)
{
    if (texture != NULL)
    {
        glDeleteTextures(1, &texture->id);

        if (texture->data != NULL)
        {
            stbi_image_free(texture->data);
        }

        free(texture);
    }
}

void texture_bind(const Texture *texture)
{
    if (texture != NULL)
        glBindTexture(GL_TEXTURE_2D, texture->id);
    else
        glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_use(const Texture *texture, unsigned int slot)
{
    if (texture != NULL)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }
}

void texture_set_pixels(const Texture *texture, int width, int height, const void *pixels)
{
    if (texture != NULL)
    {
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    }
}
