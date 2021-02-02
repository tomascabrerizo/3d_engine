#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>
#include "math.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "defines.h"
#include "renderable.h"
#include "renderer.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

struct GameState {
    
    SDL_Window* window;
    bool running = true;

    //TODO(tomi): Check the actual number of keys scancodes/keysym when libsdl.org works!
    //Keyboar input
    bool keys[1024];
    //Mouse 
    float mouse_offset_x = 0;
    float mouse_offset_y = 0;
    Camera camera;

    uint32_t shaders[MAX_SHADERS_COUNT];
    Texture game_textures[MAX_TEXTURE_COUT];
    Material game_materials[MAX_MATERIAL_COUNT];
    Mesh game_meshes[MAX_MESHES_COUNT];

    DirLight light_backpack;
    
    Renderable ren_tree[TREE_COUNT];
    Renderable ren_terrain;

    Renderer renderer;
};

#endif
