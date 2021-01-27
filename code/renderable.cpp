#include <GL/glew.h>
#include <assert.h>
#include "renderable.h"
#include "game_state.h"
#include "shader.h"
#include <stdio.h>

Renderable renderable_create(MeshesIndex mesh_index, MaterialIndex material_index)
{
    assert(mesh_index < MAX_MESHES_COUNT);
    assert(material_index < MAX_MATERIAL_COUNT);
    Renderable res = {};
    res.mesh_index[0] = mesh_index;
    res.material_index[0] = material_index;
    res.has_texture = true;
    return res;
}

Renderable renderable_create(MeshesIndex mesh_index, v3 color)
{
    assert(mesh_index < MAX_MESHES_COUNT);
    Renderable res = {};
    res.mesh_index[0] = mesh_index;
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
    if(ren.has_texture)
    {
        Material material = gs->game_materials[ren.material_index[0]];
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
    glBindVertexArray(gs->game_meshes[ren.mesh_index[0]].vao);
    glDrawArrays(GL_TRIANGLES, 0, gs->game_meshes[ren.mesh_index[0]].vertices_count);
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
