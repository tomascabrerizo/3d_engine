#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define MAX_TEXTURE_COUT 32
#define MAX_MATERIAL_COUNT 32
#define MAX_MESHES_COUNT 32
#define MAX_LIGHTS_COUNT 32

//Game textures
enum TextureIndex {
    TEXTURE_EMPTY = 0,
    TEXTURE_CUBE,
    TEXTURE_BOX,
    TEXTURE_WOODBOX,
    TEXTURE_WOODBOX_SPECULAR,

    TEXTURE_COUNT,
};

enum MeshesIndex {
    MESH_EMPTY = 0,
    MESH_CUBE,

    MESH_COUNT
};

enum MaterialIndex{
    MATERIAL_EMPTY= 0,
    MATERIAL_TEST,

    MATERIAL_COUNT,
};

enum LightIndex{
    LIGHT_EMPTY= 0,
    LIGHT_TEST,

    LIGHT_COUNT,
};

typedef struct
{
    char* content;
    uint32_t size;
    
} file_info;

file_info read_entire_file(const char* path);
char* concat_string(const char* s0, const char* s1, char* s);

#endif
