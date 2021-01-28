#include <GL/glew.h>
#include <stdio.h>
#include <stddef.h>
#include <vector>
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

    free(mesh->vertices);
    free(mesh->indices);
}

MeshIndex mesh_create(Vertex* vertices, uint32_t vertices_count, uint32_t* indices, uint32_t indices_count, GameState* gs)
{
    //TODO(tomi):Assert tha vertices and indices enter in gs->game_meshes and gs->game_indices 
    MeshIndex ms = {gs->last_mesh_index+1, 1};
    gs->game_meshes[ms.f_index].vertices = (Vertex*)malloc(vertices_count*sizeof(Vertex));
    for(uint32_t i = 0; i < vertices_count; ++i)
    {
        gs->game_meshes[ms.f_index].vertices[i] = vertices[i]; 
    }
    gs->game_meshes[ms.f_index].indices = (uint32_t*)malloc(indices_count*sizeof(uint32_t));
    for(uint32_t i = 0; i < indices_count; ++i)
    {
        gs->game_meshes[ms.f_index].indices[i] = indices[i]; 
    }
    mesh_initialize(&gs->game_meshes[ms.f_index]);
    return ms;
}

MeshIndex mesh_load_from_obj(const char* path, GameState* gs)
{
    //TODO(tomi):Assert tha vertices and indices enter in gs->game_meshes and gs->game_indices 
    MeshIndex ms = {gs->last_mesh_index+1, 0};
    
    FILE* data = fopen(path, "r");
    if(!data)
    {
        printf("cannot find file (%s)\n", path);
        return {0 , 0};
    }
    
    char line[512];
    //TODO(tomi):Try to implements obj parser without StringView and std::vector
    std::vector<v3> vertices;
    std::vector<v2> text_coords;
    std::vector<v3> normals;
    
    std::vector<Vertex> vertexs;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> mesh_index;

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
            //v3_dump(new_v3(f0, f1, f2));
            vertices.push_back(new_v3(f0, f1, f2));
        }
        else if(sv_equals(header, "vt"))
        {
            String_View s0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s1 = sv_trim(sv_line);
            float f0 = sv_to_float(s0);
            float f1 = sv_to_float(s1);
            text_coords.push_back(new_v2(f0, f1));
        }
        else if(sv_equals(header, "vn"))
        {
            String_View s0 = sv_chop_by_delim(&sv_line, ' ');
            String_View s1 = sv_chop_by_delim(&sv_line, ' ');
            String_View s2 = sv_trim(sv_line);
            float f0 = sv_to_float(s0);
            float f1 = sv_to_float(s1);
            float f2 = sv_to_float(s2);
            normals.push_back(new_v3(f0, f1, f2));
        }
    }
    
    fseek(data, 0, SEEK_SET);

    while(fgets(line, sizeof(line), data))
    {
        String_View sv_line = sv_trim(cstr_as_sv(line));
        if(sv_empty(sv_line)) continue;  
        String_View header = sv_trim(sv_chop_by_delim(&sv_line, ' '));

        if(sv_equals(header, "o"))
        {
            mesh_index.push_back(gs->last_mesh_index + 1);
            gs->last_mesh_index++;
            ms.count++;
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
            
            indices.push_back(v0_index);
            indices.push_back(v1_index);
            indices.push_back(v2_index);

            Vertex new_vertex0 = new_vertex(vertices[v0_index], text_coords[vt0_index], normals[vn0_index]);
            Vertex new_vertex1 = new_vertex(vertices[v1_index], text_coords[vt1_index], normals[vn1_index]);
            Vertex new_vertex2 = new_vertex(vertices[v2_index], text_coords[vt2_index], normals[vn2_index]);
            
            vertexs.push_back(new_vertex0);
            vertexs.push_back(new_vertex1);
            vertexs.push_back(new_vertex2);
        }
    }
    
    for(uint32_t index: mesh_index)
    {
        gs->game_meshes[index].vertices = (Vertex*)malloc(vertexs.size()*sizeof(Vertex));
        gs->game_meshes[index].vertices_count = vertexs.size();
        
        gs->game_meshes[index].indices = (uint32_t*)malloc(indices.size()*sizeof(uint32_t));
        gs->game_meshes[index].indices_count = indices.size();
        
        for(uint32_t i = 0; i < vertexs.size(); ++i)
        {
            gs->game_meshes[index].vertices[indices[i]] = vertexs[i];
        }
        for(uint32_t i = 0; i < indices.size(); ++i)
        {
            gs->game_meshes[index].indices[i] = indices[i];
        }
        mesh_initialize(&gs->game_meshes[index]);
    }

    printf("FILE:'%s' loaded!\n", path);
    return ms;
}
