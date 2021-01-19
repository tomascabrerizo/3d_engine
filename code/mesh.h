#ifndef MESH_H
#define MESH_H
#include "math.h"

struct Mesh {
    uint32_t vao;
    uint32_t vertices_count;
    uint32_t tex_coords_count;
};

Mesh mesh_create(float* vertices, uint32_t v_count, float* tex_coords, uint32_t t_count);

#endif
