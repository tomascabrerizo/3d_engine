#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>
#include "math.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "utils.h"
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
    Material game_materials[MAX_MATERIAL_COUNT];
    Mesh game_meshes[MAX_MESHES_COUNT];

    DirLight dir_light;
    PointLight point_lights[MAX_POINT_LIGHTS];

    Renderable ren_cubes[10];
    Renderable ren_lights[MAX_POINT_LIGHTS];

};

#endif
