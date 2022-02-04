#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include "texture.h"

typedef struct Framebuffer
{
    unsigned int id;
    int width, height;
    Texture *texture;
} Framebuffer;

Framebuffer *framebuffer_create(int width, int height);
void framebuffer_destroy(Framebuffer *fb);

void framebuffer_bind(const Framebuffer *fb);

#endif // FRAMEBUFFER_H