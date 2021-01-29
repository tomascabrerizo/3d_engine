#include <GL/glew.h>
#include <assert.h>
#include "renderable.h"
#include "game_state.h"
#include "shader.h"
#include <stdio.h>

Renderable renderable_create(MeshIndex mesh_index, MaterialIndex material_index)
{
    Renderable res = {};
    res.mesh_index = mesh_index;
    res.material_index = material_index;
    res.has_texture = true;
    return res;
}

Renderable renderable_create(MeshIndex mesh_index, v3 color)
{
    Renderable res = {};
    res.mesh_index = mesh_index;
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
    
    Mesh mesh = gs->game_meshes[ren.mesh_index];
    glBindVertexArray(gs->game_meshes[ren.mesh_index].vao);
    glDrawElements(GL_TRIANGLES, mesh.indices_count, GL_UNSIGNED_INT, 0);
}
