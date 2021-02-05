#ifndef DEFINES_H
#define DEFINES_H

#define TREE_COUNT 1024 
#define GRASS_COUNT 256 
#define FERN_COUNT 512 

#define MAX_TEXTURE_COUT 32
#define MAX_MATERIAL_COUNT 32
#define MAX_MESHES_COUNT 32 
#define MAX_SHADERS_COUNT 32

enum ShadersIndex {
    SHADER_EMPTY = 0,
    SHADER_COLOR,
    SHADER_TEXTURE,
    SHADER_TERRAIN,

    SHADER_COUNT,
};

enum MeshIndex {
    MESH_EMPTY = 0,
    MESH_TERRAIN,
    MESH_TREE,
    MESH_GRASS,
    MESH_FERN,

    MESH_COUNT,
};

//NOTE(tomi):Game textures
enum TextureIndex {
    TEXTURE_EMPTY = 0,
    TEXTURE_SPECULAR,
    TEXTURE_TREE,
    TEXTURE_TERRAIN,
    TEXTURE_BLEND_MAP,
    TEXTURE_FLOWER,
    TEXTURE_MUD,
    TEXTURE_PATH,
    TEXTURE_GRASS,
    TEXTURE_FERN,

    TEXTURE_COUNT,
};

//NOTE(tomi):Game Materials
enum MaterialIndex{
    MATERIAL_EMPTY= 0,
    MATERIAL_TREE,
    MATERIAL_TERRAIN,
    MATERIAL_GRASS,
    MATERIAL_FERN,

    MATERIAL_COUNT,
};

#endif
