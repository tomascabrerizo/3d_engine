#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"
#include "defines.h"
#include "renderable.h"

#define TERRAIN_SIZE 800
#define TERRAIN_VERTEX_COUNT 128 
#define TERRAIN_INDICES_COUNT (6 * (TERRAIN_VERTEX_COUNT - 1))

struct MultiTexture
{
    TextureIndex r_texture; 
    TextureIndex g_texture; 
    TextureIndex b_texture; 

    TextureIndex blend_texture; 
};

struct Terrain 
{
    Renderable ren;
    MultiTexture multi_texture;
};

Terrain terrain_create(uint32_t mesh_index, uint32_t material_index, MultiTexture multi_texture);
Mesh terrain_generate();

#endif
