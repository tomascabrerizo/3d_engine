#include <stdio.h>
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
    renderer->render_queue[index].has_alpha = ren[0].has_alpha;
    renderer->render_queue[index].fake_light = ren[0].fake_light;
    renderer->render_queue[index].is_terrain = false;
    renderer->index++; 
}


void renderer_add(Renderer* renderer, Terrain* terrain, uint32_t terrain_count, uint32_t shader, GameState* gs)
{
    assert(renderer->index < MAX_MESHES_COUNT);
    Renderable* ren = &terrain->ren;
    uint32_t index =  renderer->index;
    renderer->render_queue[index].renderable = ren; 
    renderer->render_queue[index].shader = gs->shaders[shader]; 
    renderer->render_queue[index].count = terrain_count;
    renderer->render_queue[index].vao = gs->game_meshes[ren->mesh_index].vao;
    renderer->render_queue[index].has_alpha = ren[0].has_alpha;
    renderer->render_queue[index].fake_light = ren[0].fake_light;
    renderer->render_queue[index].is_terrain = true;
    renderer->render_queue[index].multi_texture = terrain->multi_texture;
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
        bool has_alpha = renderer->render_queue[i].has_alpha;
        bool fake_light = renderer->render_queue[i].fake_light;
        bool is_terrain = renderer->render_queue[i].is_terrain;
        if(has_alpha)
        {
            glDisable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        
        shader_use_program(shader);
        glBindVertexArray(vao);
        shader_set_bool(shader, "fake_light", fake_light); 
        shader_set_material(shader, "material", material);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gs->game_textures[material.texture_index].id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gs->game_textures[material.texture_specular_index].id);

        //TODO(tomi)Maybe try to use functions pointer to diferents renderables types
        if(is_terrain)
        {
            MultiTexture mt = renderer->render_queue[i].multi_texture;
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gs->game_textures[mt.r_texture].id);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, gs->game_textures[mt.g_texture].id);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, gs->game_textures[mt.b_texture].id);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, gs->game_textures[mt.blend_texture].id);
        }

        for(uint32_t j = 0; j < renderer->render_queue[i].count; ++j)
        {
            Renderable* ren = &renderer->render_queue[i].renderable[j];
            //TODO(tomi):See if is better update model matrix inside render
            renderable_update(ren);
            shader_set_m4(shader, "model", ren->model);
            Mesh mesh = gs->game_meshes[ren->mesh_index];
            glDrawElements(GL_TRIANGLES, mesh.indices_count, GL_UNSIGNED_INT, 0);
        }
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

    }
    
}
