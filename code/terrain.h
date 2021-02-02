#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"

#define TERRAIN_SIZE 800
#define TERRAIN_VERTEX_COUNT 128 
#define TERRAIN_INDICES_COUNT (6 * (TERRAIN_VERTEX_COUNT - 1))

//TODO(tomi):Move this function to terrain.cpp
Mesh terrain_generate()
{
    Mesh terrain = {};
    terrain.vertices_count = TERRAIN_VERTEX_COUNT * TERRAIN_VERTEX_COUNT;
    terrain.indices_count = TERRAIN_INDICES_COUNT * TERRAIN_INDICES_COUNT;

    terrain.vertices = (Vertex*)malloc(terrain.vertices_count*sizeof(Vertex));
    terrain.indices = (uint32_t*)malloc(terrain.indices_count*sizeof(uint32_t));

    //TODO(tomi):Calculate Normals
    for(uint32_t z = 0; z < TERRAIN_VERTEX_COUNT; ++z)
    {
        for(uint32_t x = 0; x < TERRAIN_VERTEX_COUNT; ++x)
        {
            uint32_t index = (z*TERRAIN_VERTEX_COUNT)+x;
            float terrain_x = (float)x / (float)(TERRAIN_VERTEX_COUNT - 1);
            float terrain_z = (float)z / (float)(TERRAIN_VERTEX_COUNT - 1);
            
            v3 current_pos = new_v3(terrain_x*TERRAIN_SIZE, 0, terrain_z * TERRAIN_SIZE);
            terrain.vertices[index].position  = current_pos - new_v3(TERRAIN_SIZE/2, 0, TERRAIN_SIZE/2);
            terrain.vertices[index].tex_coord = new_v2(terrain_x, terrain_z) * TERRAIN_VERTEX_COUNT;
            
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
    printf("TERRAIN: loaded!\n");
    return terrain;
}

float* terrain_calculate_height_map(uint32_t x, uint32_t z)
{
    return 0;
}

#endif
