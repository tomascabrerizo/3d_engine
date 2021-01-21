#ifndef MESH_H
#define MESH_H
#include "math.h"

struct Mesh {
    uint32_t vao;
    uint32_t vertices_count;
    uint32_t tex_coords_count;
    uint32_t normals_count;
};

Mesh mesh_create(float* vertices, uint32_t vertices_size, float* tex_coords, uint32_t tex_coords_size, float* normals, uint32_t normals_size);

#endif
