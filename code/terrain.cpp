#include <stdio.h>
#include <assert.h>
#include "terrain.h"


Terrain terrain_create(uint32_t mesh_index, uint32_t material_index, MultiTexture multi_texture)
{
    Terrain terrain = {};
    terrain.ren = renderable_create((MeshIndex)mesh_index, (MaterialIndex)material_index); 
    terrain.multi_texture = multi_texture;
    return terrain;
}

Mesh terrain_generate(bitmap height_map)
{
    Mesh terrain = {};
    
    assert(height_map.bytes_per_pixel == 4);
    const uint32_t terrain_vertex_count = height_map.height;
    const uint32_t terrain_indices_count = (6*(terrain_vertex_count-1)); 
    
    terrain.vertices_count = terrain_vertex_count * terrain_vertex_count;
    terrain.indices_count = terrain_indices_count * terrain_indices_count;
    
    terrain.vertices = (Vertex*)malloc(terrain.vertices_count*sizeof(Vertex));
    terrain.indices = (uint32_t*)malloc(terrain.indices_count*sizeof(uint32_t));
    
    for(uint32_t z = 0; z < terrain_vertex_count; ++z)
    {
        for(uint32_t x = 0; x < terrain_vertex_count; ++x)
        {
            uint32_t index = (z*terrain_vertex_count)+x;
            float terrain_x = (float)x / (float)(terrain_vertex_count-1);
            float terrain_z = (float)z / (float)(terrain_vertex_count-1);
            
            v3 current_pos = new_v3(
                    terrain_x*TERRAIN_SIZE, 
                    get_height_form_map(x, z, height_map), 
                    terrain_z * TERRAIN_SIZE
            );
            
            //printf("(x:%d, z:%d)height: %f\n", x, z, get_height_form_map(x, z, &height_map));

            terrain.vertices[index].position  = current_pos - new_v3(TERRAIN_SIZE/2, 0, TERRAIN_SIZE/2);
            terrain.vertices[index].tex_coord = new_v2(terrain_x, terrain_z);

            float terrain_new_x = (float)(1 / (float)(TERRAIN_VERTEX_COUNT - 1))*TERRAIN_SIZE;
            float terrain_new_z = (float)(1 / (float)(TERRAIN_VERTEX_COUNT - 1))*TERRAIN_SIZE;
            //TODO(tomi):Make this normal generator better
            v3 v0 = normalize(new_v3(terrain_new_x, get_height_form_map(x+1, z, height_map), 0) - current_pos);
            v3 v1 = normalize(new_v3(0, get_height_form_map(x, z+1, height_map), terrain_new_z) - current_pos);

            terrain.vertices[index].normal = normalize(cross(v0, v1));
        }

    }
    
    uint32_t index = 0;
    for(uint32_t z = 0; z < terrain_vertex_count-1; ++z)
    {
        for(uint32_t x = 0; x < terrain_vertex_count-1; ++x)
        {
            uint32_t top_left = (z * terrain_vertex_count) + x;
            uint32_t top_right = top_left + 1;
            uint32_t bottom_left = ((z+1)*terrain_vertex_count) + x;
            uint32_t bottom_right = bottom_left + 1;
            terrain.indices[index++] = top_left;
            terrain.indices[index++] = bottom_left;
            terrain.indices[index++] = top_right;
            terrain.indices[index++] = top_right;
            terrain.indices[index++] = bottom_left;
            terrain.indices[index++] = bottom_right;
        }
    }

    mesh_initialize(&terrain);
    printf("TERRAIN: Generated!\n");
    return terrain; 
}

float get_height_form_map(uint32_t x, uint32_t z, bitmap map)
{
    if(x < 0 || x > map.height || z < 0 || z > map.height)
    {
        return 0;
    }
    uint32_t index = (z*map.height)+x;
    uint32_t color_alpha = ((uint32_t*)map.pixels)[index];
    
    //uint32_t color = (color_alpha >> 8) & 0xFFFFFF; 
    uint32_t color = color_alpha & 0xFFFFFF; 
    
    float result = (float)color / (float)MAP_MAX_COLOR;
    float range = (MAP_MAX_TOP - MAP_MAX_BOTTOM);
    return (result * range) + MAP_MAX_BOTTOM;
}

Mesh terrain_generate()
{
    Mesh terrain = {};
    terrain.vertices_count = TERRAIN_VERTEX_COUNT * TERRAIN_VERTEX_COUNT;
    terrain.indices_count = TERRAIN_INDICES_COUNT * TERRAIN_INDICES_COUNT;

    terrain.vertices = (Vertex*)malloc(terrain.vertices_count*sizeof(Vertex));
    terrain.indices = (uint32_t*)malloc(terrain.indices_count*sizeof(uint32_t));

    for(uint32_t z = 0; z < TERRAIN_VERTEX_COUNT; ++z)
    {
        for(uint32_t x = 0; x < TERRAIN_VERTEX_COUNT; ++x)
        {
            uint32_t index = (z*TERRAIN_VERTEX_COUNT)+x;
            float terrain_x = (float)x / (float)(TERRAIN_VERTEX_COUNT - 1);
            float terrain_z = (float)z / (float)(TERRAIN_VERTEX_COUNT - 1);
            
            v3 current_pos = new_v3(terrain_x*TERRAIN_SIZE, 0, terrain_z * TERRAIN_SIZE);
            terrain.vertices[index].position  = current_pos - new_v3(TERRAIN_SIZE/2, 0, TERRAIN_SIZE/2);
            terrain.vertices[index].tex_coord = new_v2(terrain_x, terrain_z);
            
            float terrain_new_x = (float)(1 / (float)(TERRAIN_VERTEX_COUNT - 1))*TERRAIN_SIZE;
            float terrain_new_z = (float)(1 / (float)(TERRAIN_VERTEX_COUNT - 1))*TERRAIN_SIZE;
            //TODO(tomi):Make this normal generator better
            v3 v0 = normalize(new_v3(terrain_new_x, 0, 0) - current_pos);
            v3 v1 = normalize(new_v3(0, 0, terrain_new_z) - current_pos);

            terrain.vertices[index].normal = normalize(cross(v0, v1));
        }
    }
    
    uint32_t index = 0;
    for(uint32_t z = 0; z < TERRAIN_VERTEX_COUNT-1; ++z)
    {
        for(uint32_t x = 0; x < TERRAIN_VERTEX_COUNT-1; ++x)
        {
            uint32_t top_left = (z * TERRAIN_VERTEX_COUNT) + x;
            uint32_t top_right = top_left + 1;
            uint32_t bottom_left = ((z+1)*TERRAIN_VERTEX_COUNT) + x;
            uint32_t bottom_right = bottom_left + 1;
            terrain.indices[index++] = top_left;
            terrain.indices[index++] = bottom_left;
            terrain.indices[index++] = top_right;
            terrain.indices[index++] = top_right;
            terrain.indices[index++] = bottom_left;
            terrain.indices[index++] = bottom_right;
        }
    }

    mesh_initialize(&terrain);
    printf("TERRAIN: Generated!\n");
    return terrain;
}
