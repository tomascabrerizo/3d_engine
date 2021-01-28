#ifndef MESH_H
#define MESH_H
#include "math.h"
#include "utils.h"

struct GameState;

struct Vertex {
    v3 position;
    v2 tex_coord;
    v3 normal;
};

Vertex new_vertex(v3 position, v2 tex_coord, v3 normal);

//TODO(tomi):This should be the only MeshIndex, (remove the MeshIndex enum)
struct MeshIndex {
    uint32_t f_index;
    uint32_t count;
};

struct Mesh {
    uint32_t vao;

    Vertex* vertices;
    uint32_t vertices_count;
    uint32_t* indices;
    uint32_t indices_count;
};

MeshIndex mesh_create(Vertex* vertices, uint32_t vertices_count, uint32_t* indices, uint32_t indices_count, GameState* gs);
MeshIndex mesh_load_from_obj(const char* path, GameState* gs);
void mesh_initialize(Mesh* mesh);

#endif
