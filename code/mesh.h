#ifndef MESH_H
#define MESH_H
#include <vector>
#include "math.h"

struct Vertex {
    v3 position;
    v3 normal;
    v2 tex_coord;
};

Vertex new_vertex(v3 position, v3 normal, v2 tex_coord);

struct Mesh {
    uint32_t vao;
    uint32_t vertices_count;
    uint32_t tex_coords_count;
    uint32_t normals_count;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    //NOTE(tomi):IMPORTANT this is texture index, from de array of texture not texture coords
    std::vector<uint32_t> texture_index;
};

Mesh mesh_create(std::vector<Vertex> vertice, std::vector<uint32_t> indices, std::vector<uint32_t> texture_index);
Mesh mesh_create(float* vertices, uint32_t vertices_size, float* tex_coords, uint32_t tex_coords_size, float* normals, uint32_t normals_size);

#endif
