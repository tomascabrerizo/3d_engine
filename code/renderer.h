#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "defines.h"
#include "renderable.h"

struct GameState;

struct RenderableInfo 
{
    Renderable* renderable;
    uint32_t vao;
    uint32_t shader;
    uint32_t count;
};

struct Renderer 
{
    RenderableInfo render_queue[MAX_MESHES_COUNT];
    uint32_t index = 0;
};

void renderer_add(Renderer* renderer, Renderable* ren, uint32_t ren_count, uint32_t shader, GameState* gs);
void renderer_render(Renderer* renderer, GameState* gs);

#endif
