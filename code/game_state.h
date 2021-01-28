#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>
#include <vector>
#include "math.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "defines.h"
#include "renderable.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct GameState {
    
    SDL_Window* window;
    bool running = true;

    //TODO(tomi): Check the actual number of keys scancodes/keysym when libsdl.org works!
    //Keyboar input
    bool keys[1024];
    //Mouse 
    float mouse_offset_x = 0;
    float mouse_offset_y = 0;
    
    uint32_t shader_program;
    uint32_t shader_program2;

    Camera camera;

    Texture game_textures[MAX_TEXTURE_COUT];
    uint32_t last_texture_index = TEXTURE_COUNT - 1;

    Material game_materials[MAX_MATERIAL_COUNT];
    uint32_t last_material_index = MATERIAL_COUNT - 1;
    
    Mesh game_meshes[MAX_MESHES_COUNT];
    uint32_t last_mesh_index = MESH_COUNT - 1;

    DirLight light_backpack;
    Renderable ren_backpack;
};

#endif
