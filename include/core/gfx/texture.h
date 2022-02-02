#ifndef _TEXTURE_H_
#define _TEXTURE_H_

typedef struct Texture
{
    int width, height;
    int bpp;
    unsigned int id;
    void *data;
    char path[128];
} Texture;

Texture *texture_create(int width, int height);
Texture *texture_load(const char *path, int keep_data);
void texture_destroy(Texture *texture);

void texture_bind(const Texture *texture);
void texture_use(const Texture *texture, unsigned int slot);
void texture_set_pixels(const Texture *texture, int width, int height, const void *pixels);

#endif // _TEXTURE_H_
