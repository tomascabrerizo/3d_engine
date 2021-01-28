#include <GL/glew.h>
#include <assert.h>
#include "renderable.h"
#include "game_state.h"
#include "shader.h"
#include <stdio.h>

Renderable renderable_create(MeshIndex mesh_index, MaterialIndex material_index)
{
    assert(mesh_index.f_index + mesh_index.count< REN_MESH_INDEX_MAX_SIZE);
    assert(material_index < MAX_MATERIAL_COUNT);
    Renderable res = {};
    res.mesh_index_count = mesh_index.count;
    uint32_t mi = mesh_index.f_index;
    for(uint32_t i = 0; i < mesh_index.count; ++i)
    {
        res.mesh_index[i] = mi++;        
    }
    res.material_index = material_index;
    res.has_texture = true;
    return res;
}

Renderable renderable_create(MeshIndex mesh_index, v3 color)
{
    assert(mesh_index.f_index + mesh_index.count< REN_MESH_INDEX_MAX_SIZE);
    Renderable res = {};
    res.mesh_index_count = mesh_index.count;
    uint32_t mi = mesh_index.f_index;
    for(uint32_t i = 0; i < mesh_index.count; ++i)
    {
        res.mesh_index[i] = mi++;        
    }
    res.color = color;
    res.has_texture = false;
    return res;
}

void renderable_update(Renderable* ren)
{
    m4 r = rotate_x(ren->rotate.x)*rotate_y(ren->rotate.y)*rotate_z(ren->rotate.z);
    m4 s = scale(ren->scale);
    m4 t = translate(ren->pos);
    ren->model = t*r*s;

}

void renderable_render(const Renderable& ren, uint32_t shader, GameState* gs)
{

    shader_use_program(shader);
    shader_set_m4(shader, "model", ren.model);
    //TODO(tomi):Try to no use if statement here, i think is not necesary
    if(ren.has_texture)
    {
        Material material = gs->game_materials[ren.material_index];
        shader_set_material(shader, "material", material);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gs->game_textures[material.texture_index].id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gs->game_textures[material.texture_specular_index].id);
    }
    else
    {
        shader_set_v3(shader, "ren_color", ren.color);
    }
    
    for(uint32_t i = 0; i < ren.mesh_index_count; ++i)
    {
        uint32_t index = ren.mesh_index[i];
        Mesh mesh = gs->game_meshes[index];
        glBindVertexArray(gs->game_meshes[index].vao);
        glDrawElements(GL_TRIANGLES, mesh.indices_count, GL_UNSIGNED_INT, 0);
    }
}
