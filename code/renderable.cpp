#include <GL/glew.h>
#include <assert.h>
#include "renderable.h"
#include "shader.h"

Renderable renderable_create(MeshesIndex mesh_index, TextureIndex texture_index, TextureIndex texture_specular_index, MaterialIndex material_index)
{
    assert(mesh_index < MAX_MESHES_COUNT);
    assert(texture_index < MAX_MESHES_COUNT);
    Renderable res = {};
    res.mesh_index = mesh_index;
    res.texture_index = texture_index;
    res.texture_specular_index = texture_specular_index;
    res.material_index = material_index;
    res.has_texture = true;
    return res;
}

Renderable renderable_create(MeshesIndex mesh_index, v3 color)
{
    Renderable res = {};
    res.mesh_index = mesh_index;
    res.texture_index = TEXTURE_EMPTY;
    res.has_texture = false;
    res.color = color;
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
    shader_set_bool(shader, "has_texture", ren.has_texture);
    shader_set_v3(shader, "ren_color", ren.color);
    shader_set_material(shader, "material", gs->game_materials[ren.material_index]);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gs->game_textures[ren.texture_index].id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gs->game_textures[ren.texture_specular_index].id);
    
    glBindVertexArray(gs->game_meshes[ren.mesh_index].vao);
    glDrawArrays(GL_TRIANGLES, 0, gs->game_meshes[ren.mesh_index].vertices_count);
}
