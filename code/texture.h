#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture {
    uint32_t id;
    uint32_t w;
    uint32_t h;
};

Texture texture_create(const char* path);

#endif
