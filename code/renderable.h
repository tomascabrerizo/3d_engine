#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <stdint.h>
#include "math.h"
#include "game_state.h"

struct Renderable
{
    uint32_t mesh_index;
    uint32_t texture_index;
    v3 pos;
    v3 scale = {1.0f, 1.0f, 1.0f}; //if scale init to 0 the object will not have size
    v3 rotate;
    m4 model;
};

Renderable renderable_create(MeshesIndex mesh_index, TextureIndex texture_index);
void renderable_update(Renderable* ren);
void renderable_render(const Renderable& ren, uint32_t shader, GameState* gs);

#endif
