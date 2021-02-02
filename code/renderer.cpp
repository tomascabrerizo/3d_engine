#include <GL/glew.h>
#include <assert.h>
#include "game_state.h"
#include "renderer.h"
#include "material.h"
#include "shader.h"

void renderer_add(Renderer* renderer, Renderable* ren, uint32_t ren_count, uint32_t shader, GameState* gs)
{
    assert(renderer->index < MAX_MESHES_COUNT);
    uint32_t index =  renderer->index;
    renderer->render_queue[index].renderable = ren; 
    renderer->render_queue[index].shader = gs->shaders[shader]; 
    renderer->render_queue[index].count = ren_count;
    renderer->render_queue[index].vao = gs->game_meshes[ren->mesh_index].vao;
    renderer->index++; 
}

void renderer_render(Renderer* renderer, GameState* gs)
{
    for(uint32_t i = 0; i < renderer->index; ++i)
    {
        uint32_t vao = renderer->render_queue[i].vao;
        uint32_t shader =  renderer->render_queue[i].shader;
        uint32_t material_index = renderer->render_queue[i].renderable[0].material_index;
        Material material = gs->game_materials[material_index];

        shader_use_program(shader);
        glBindVertexArray(vao);
        
        shader_set_material(shader, "material", material);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gs->game_textures[material.texture_index].id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gs->game_textures[material.texture_specular_index].id);

        for(uint32_t j = 0; j < renderer->render_queue[i].count; ++j)
        {
            Renderable* ren = &renderer->render_queue[i].renderable[j];
            //TODO(tomi):See if is better update model matrix inside render
            renderable_update(ren);
            shader_set_m4(shader, "model", ren->model);
            Mesh mesh = gs->game_meshes[ren->mesh_index];
            glDrawElements(GL_TRIANGLES, mesh.indices_count, GL_UNSIGNED_INT, 0);
        }

    }
    
}