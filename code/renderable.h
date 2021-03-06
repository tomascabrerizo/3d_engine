#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <stdint.h>
#include "math.h"
#include "utils.h"
#include "defines.h"
#include "mesh.h"

struct GameState;

struct Renderable
{
    uint32_t mesh_index;
    uint32_t material_index;
    
    v3 color;
    
    v3 pos;
    v3 scale = {1.0f, 1.0f, 1.0f}; //if scale init to 0 the object will not have size
    v3 rotate;
    m4 model;
    
    bool has_alpha = false;
    bool fake_light = false;
};

Renderable renderable_create(MeshIndex mesh_index, MaterialIndex material_index);
void renderable_update(Renderable* ren);
void renderable_render(const Renderable& ren, uint32_t shader, GameState* gs);

#endif
