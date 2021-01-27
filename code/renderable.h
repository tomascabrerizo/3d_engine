#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <stdint.h>
#include "math.h"
#include "utils.h"
#include "defines.h"

struct GameState;

#define MESH_INDEX_MAX_SIZE 256
#define MATERIAL_INDEX_MAX_SIZE 256

struct Renderable
{
    uint32_t mesh_index[MESH_INDEX_MAX_SIZE];
    uint32_t material_index[MATERIAL_INDEX_MAX_SIZE];
    
    v3 color;
    
    v3 pos;
    v3 scale = {1.0f, 1.0f, 1.0f}; //if scale init to 0 the object will not have size
    v3 rotate;
    m4 model;

    bool has_texture = false;
};

Renderable renderable_create(MeshesIndex mesh_index, v3 color);
Renderable renderable_create(MeshesIndex mesh_index, MaterialIndex material_index);
void renderable_update(Renderable* ren);
void renderable_render(const Renderable& ren, uint32_t shader, GameState* gs);

#endif
