#include <GL/glew.h>
#include <stdio.h>
#include <stddef.h>
#include "mesh.h"

#define VERTEX_SIZE 12
#define TEX_COORD_SIZE 8
#define NORMAL_SIZE 12

Vertex new_vertex(v3 position, v2 tex_coord, v3 normal)
{
    Vertex vertex = {};
    vertex.position = position;
    vertex.tex_coord = tex_coord;
    vertex.normal = normal;
    return vertex;
}

Mesh mesh_create(float* vertices, uint32_t vertices_size, float* tex_coords, uint32_t tex_coords_size, float* normals, uint32_t normals_size)
{
    Mesh new_mesh = {};
    new_mesh.vertices_count = vertices_size / VERTEX_SIZE;
    new_mesh.tex_coords_count = tex_coords_size / TEX_COORD_SIZE;
    new_mesh.normals_count = normals_size / NORMAL_SIZE;
    
    uint32_t vbo, tbo, nbo;
    glGenVertexArrays(1, &new_mesh.vao);
    glBindVertexArray(new_mesh.vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, tex_coords_size, tex_coords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2* sizeof(float), 0);
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals_size, normals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), 0);
    glEnableVertexAttribArray(2);
    return new_mesh;
}

Mesh mesh_create(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
    Mesh new_mesh = {};
    new_mesh.vertices = vertices;
    new_mesh.indices = indices;

    uint32_t vbo, ebo;
    glGenVertexArrays(1, &new_mesh.vao);
    glBindVertexArray(new_mesh.vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

    return new_mesh;
}


void mesh_initialize(Mesh* mesh)
{
    uint32_t vbo, ebo;
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size()*sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size()*sizeof(uint32_t), &mesh->indices[0], GL_STATIC_DRAW);
}
