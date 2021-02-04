#include <GL/glew.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "mesh.h"
#include "game_state.h"

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

void mesh_initialize(Mesh* mesh)
{
    uint32_t vbo, ebo;
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices_count*sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_count*sizeof(uint32_t), mesh->indices, GL_STATIC_DRAW);
}

Mesh mesh_create(Vertex* vertices, uint32_t vertices_count, uint32_t* indices, uint32_t indices_count, GameState* gs)
{
    Mesh new_mesh = {}; 
    new_mesh.vertices_count = vertices_count;
    new_mesh.vertices = vertices; 
    new_mesh.indices_count = indices_count;
    new_mesh.indices = indices; 
    mesh_initialize(&new_mesh);
    return new_mesh;
}

//TODO(tomi):Try to parse using rex (regular expresions) insted of StringView 
//TODO(tomi):Make a new function to test speed
Mesh mesh_load_from_obj(const char* path, GameState* gs)
{
    FILE* data = fopen(path, "r");
    if(!data)
    {
        printf("cannot find file (%s)\n", path);
        return {0 , 0};
    }
    
    //NOTE(tomi):Get all the arrays sizes;
    uint32_t vertices_count    = 0;
    uint32_t text_coords_count = 0;
    uint32_t normals_count     = 0;
    uint32_t vertex_count      = 0;
    uint32_t indices_count     = 0;
    char line[512];
    while(fgets(line, sizeof(line), data))
    {
        String_View sv_line = sv_trim(cstr_as_sv(line));
        if(sv_empty(sv_line)) continue;
        String_View header = sv_trim(sv_chop_by_delim(&sv_line, ' '));
        sv_line = sv_trim(sv_line);
        if(sv_equals(header, "v"))
        {
            vertices_count++; 
        }
        else if(sv_equals(header, "vt"))
        {
            text_coords_count++;
        }
        else if(sv_equals(header, "vn"))
        {
            normals_count++;
        }
        else if(sv_equals(header, "f"))
        {
            vertex_count+=3;
            indices_count+=3;
        }
    }
    //NOTE(tomi):Alloc all the necesary memory
    v3*       vertices    = (v3*)malloc(vertices_count*sizeof(v3));
    v2*       text_coords = (v2*)malloc(text_coords_count*sizeof(v3));
    v3*       normals     = (v3*)malloc(normals_count*sizeof(v3));
    Vertex*   vertexs     = (Vertex*)malloc(vertex_count*sizeof(Vertex));
    uint32_t* indices     = (uint32_t*)malloc(indices_count*sizeof(uint32_t));
    
    uint32_t v_index  = 0;
    uint32_t vt_index = 0;
    uint32_t vn_index = 0;
    uint32_t f_index  = 0;
    fseek(data, 0, SEEK_SET);
    while(fgets(line, sizeof(line), data))
    {
        String_View sv_line = sv_trim(cstr_as_sv(line));
        if(sv_empty(sv_line)) continue;
        
        String_View header = sv_trim(sv_chop_by_delim(&sv_line, ' '));
        sv_line = sv_trim(sv_line);
        if(sv_equals(header, "v"))
        {
            String_View s0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s1 = sv_chop_by_delim(&sv_line, ' ');
            String_View s2 = sv_trim(sv_line);
            float f0 = sv_to_float(s0);
            float f1 = sv_to_float(s1);
            float f2 = sv_to_float(s2);
            vertices[v_index++] = new_v3(f0, f1, f2);
        }
        else if(sv_equals(header, "vt"))
        {
            String_View s0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s1 = sv_trim(sv_line);
            float f0 = sv_to_float(s0);
            float f1 = sv_to_float(s1);
            text_coords[vt_index++] = new_v2(f0, f1);
        }
        else if(sv_equals(header, "vn"))
        {
            String_View s0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s1 = sv_chop_by_delim(&sv_line, ' ');
            String_View s2 = sv_trim(sv_line);
            float f0 = sv_to_float(s0);
            float f1 = sv_to_float(s1);
            float f2 = sv_to_float(s2);
            normals[vn_index++] = new_v3(f0, f1, f2);
        }
        else if(sv_equals(header, "f"))
        {
            String_View s_vertex0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s_vertex1 = sv_chop_by_delim(&sv_line, ' ');
            String_View s_vertex2 = sv_trim(sv_line);
            
            String_View sv0  = sv_chop_by_delim(&s_vertex0, '/');
            String_View svt0 = sv_chop_by_delim(&s_vertex0, '/');
            String_View svn0 = sv_trim(s_vertex0);

            String_View sv1  = sv_chop_by_delim(&s_vertex1, '/');
            String_View svt1 = sv_chop_by_delim(&s_vertex1, '/');
            String_View svn1 = sv_trim(s_vertex1);
            
            String_View sv2  = sv_chop_by_delim(&s_vertex2, '/');
            String_View svt2 = sv_chop_by_delim(&s_vertex2, '/');
            String_View svn2 = sv_trim(s_vertex2);

            int v0_index = sv_to_int(sv0)-1;
            int v1_index = sv_to_int(sv1)-1;
            int v2_index = sv_to_int(sv2)-1;
            
            int vt0_index = sv_to_int(svt0)-1;
            int vt1_index = sv_to_int(svt1)-1;
            int vt2_index = sv_to_int(svt2)-1;

            int vn0_index = sv_to_int(svn0)-1;
            int vn1_index = sv_to_int(svn1)-1;
            int vn2_index = sv_to_int(svn2)-1;
            
            Vertex new_vertex0 = new_vertex(vertices[v0_index], text_coords[vt0_index], normals[vn0_index]);
            Vertex new_vertex1 = new_vertex(vertices[v1_index], text_coords[vt1_index], normals[vn1_index]);
            Vertex new_vertex2 = new_vertex(vertices[v2_index], text_coords[vt2_index], normals[vn2_index]);
            
            vertexs[f_index] = new_vertex0;
            indices[f_index++] = v0_index;
            
            vertexs[f_index] = new_vertex1;
            indices[f_index++] = v1_index;
            
            vertexs[f_index] = new_vertex2;
            indices[f_index++] = v2_index;
        }
    }
    
    Mesh new_mesh = {};
    new_mesh.vertices = (Vertex*)malloc(vertices_count*sizeof(Vertex));
    new_mesh.vertices_count = vertices_count;
    
    new_mesh.indices = (uint32_t*)malloc(indices_count*sizeof(uint32_t));
    new_mesh.indices_count = indices_count;
    
    for(uint32_t i = 0; i < indices_count; ++i)
    {
        new_mesh.vertices[indices[i]] = vertexs[i];
        new_mesh.indices[i] = indices[i];
    }
    
    mesh_initialize(&new_mesh);
    
    //NOTE(tomi):Free all allocated memory that is no needed any more
    free(vertices);   
    free(text_coords);
    free(normals);    
    free(vertexs);    
    free(indices);
    //free(new_mesh.vertices);
    //free(new_mesh.indices);

    printf("MESH: %s Loaded!\n", path);
    return new_mesh;
}
