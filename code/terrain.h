#ifndef TERRAIN_H
#define TERRAIN_H

#include "utils.h"
#include "mesh.h"
#include "defines.h"
#include "renderable.h"

#define TERRAIN_SIZE 800
#define TERRAIN_VERTEX_COUNT 128 
#define TERRAIN_INDICES_COUNT (6 * (TERRAIN_VERTEX_COUNT - 1))
#define MAP_MAX_COLOR (256*256*256)
#define MAP_MAX_TOP 40
#define MAP_MAX_BOTTOM -50

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
Mesh terrain_generate(bitmap height_map);
Mesh terrain_generate();
float get_height_form_map(uint32_t x, uint32_t z, bitmap map);

#endif
