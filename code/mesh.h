#ifndef MESH_H
#define MESH_H
#include <vector>
#include "math.h"

struct Vertex {
    v3 position;
    v2 tex_coord;
    v3 normal;
};

Vertex new_vertex(v3 position, v2 tex_coord, v3 normal);

struct Mesh {
    uint32_t vao;
    uint32_t tex_coords_count;
    uint32_t normals_count;

    Vertex* vertices;
    uint32_t vertices_count;
    uint32_t* indices;
    uint32_t indices_count;
};

Mesh mesh_create(std::vector<Vertex> vertice, std::vector<uint32_t> indices);
Mesh mesh_create(float* vertices, uint32_t vertices_size, float* tex_coords, uint32_t tex_coords_size, float* normals, uint32_t normals_size);
void mesh_initialize(Mesh* mesh);

#endif
