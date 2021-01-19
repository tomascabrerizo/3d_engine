#include <GL/glew.h>
#include "mesh.h"
#include <stdio.h>

#define VERTEX_SIZE 12
#define TEX_COORD_SIZE 8

Mesh mesh_create(float* vertices, uint32_t vertices_size, float* tex_coords, uint32_t tex_coords_size)
{
    Mesh new_mesh = {};
    new_mesh.vertices_count = vertices_size / VERTEX_SIZE;
    new_mesh.tex_coords_count = tex_coords_size / TEX_COORD_SIZE;
    
    uint32_t vbo, tbo;
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
    
    return new_mesh;
}
