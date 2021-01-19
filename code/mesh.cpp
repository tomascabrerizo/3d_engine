#include <GL/glew.h>
#include "mesh.h"

Mesh mesh_create(float* vertices, uint32_t v_count, float* tex_coords, uint32_t t_count)
{
    Mesh new_mesh = {};
    new_mesh.vertices_count = v_count;
    new_mesh.tex_coords_count = t_count;
    
    uint32_t vbo, tbo;
    glGenVertexArrays(1, &new_mesh.vao);
    glBindVertexArray(new_mesh.vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, v_count, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, t_count, tex_coords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2* sizeof(float), 0);
    glEnableVertexAttribArray(1);
    
    return new_mesh;
}
