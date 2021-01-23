#ifndef DEFINES_H
#define DEFINES_H

#define MAX_TEXTURE_COUT 32
#define MAX_MATERIAL_COUNT 32
#define MAX_MESHES_COUNT 32
#define MAX_POINT_LIGHTS 4

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

#endif
