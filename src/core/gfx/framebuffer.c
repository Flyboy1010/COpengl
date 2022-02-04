#include "core/gfx/framebuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stddef.h>
#include <stdlib.h>

Framebuffer *framebuffer_create(int width, int height)
{
    Framebuffer *fb = malloc(sizeof(Framebuffer));

    fb->width = width;
    fb->height = height;

    // create framebuffer

    glGenFramebuffers(1, &fb->id);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->id);

    // create an empty texture with the size (width * height)

    fb->texture = texture_create(width, height);

    // attach the texture to the framebuffer

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->texture->id, 0);

    // unbind the framebuffer

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // return it

    return fb;
}

void framebuffer_destroy(Framebuffer *fb)
{
    texture_destroy(fb->texture);
    glDeleteFramebuffers(1, &fb->id);
    free(fb);
}

void framebuffer_bind(const Framebuffer *fb)
{
    if (fb != NULL)
        glBindFramebuffer(GL_FRAMEBUFFER, fb->id);
    else
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
