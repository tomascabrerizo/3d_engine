#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "shader.h"
#include "math.h"
#include "cube.h"
#include "game_state.h"
#include "renderable.h"
#include "utils.h"
#include "terrain.h"

#define internal static
#define global static

internal 
void process_input(GameState* game_state)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        //TODO(tomi):Use key simbols insted of scancodes
        switch(event.type)
        {
        case SDL_QUIT: {
            game_state->running = false;
        }break;
        case SDL_KEYDOWN: {
        if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            game_state->running = false;
        }
        else
        {
            game_state->keys[event.key.keysym.scancode] = true;
        }
        }break;
        case SDL_KEYUP: {
            game_state->keys[event.key.keysym.scancode] = false;
        }break;
        case SDL_MOUSEMOTION: {
            game_state->mouse_offset_x = event.motion.xrel;
            game_state->mouse_offset_y = event.motion.yrel;
        }break;
        }
    }
}

internal
SDL_Window* initialize_platform(GameState* game_state)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
	    "3d_engine",
	    SDL_WINDOWPOS_UNDEFINED,
	    SDL_WINDOWPOS_UNDEFINED,
	    WINDOW_WIDTH, WINDOW_HEIGHT,
	    SDL_WINDOW_OPENGL
	);    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if(!gl_context)
    {
        printf("ERROR: SDL cannot create opengl context\n");
        game_state->running = false;
    }
    GLenum glew_error = glewInit();
    if(glew_error != GLEW_OK)
    {
	    printf("ERROR: initilizing glew: %s\n", glewGetErrorString(glew_error));
        game_state->running = false;
    }

    glEnable(GL_DEPTH_TEST);
    SDL_GL_SetSwapInterval(1);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    printf("OpenGL version: (%s)\n", glGetString(GL_VERSION));
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    game_state->running = true;

    return window; 
}

void game_init(GameState* game_state)
{
    //*game_state = {};
    game_state->running = true;
    game_state->window = initialize_platform(game_state);
    game_state->sky_color = new_v3(0.5, 0.5, 0.5);
    //Important Asserts
    assert(SHADER_COUNT < MAX_SHADERS_COUNT);
    assert(MESH_COUNT < MAX_MESHES_COUNT);
    assert(TEXTURE_COUNT < MAX_TEXTURE_COUT);
    assert(MATERIAL_COUNT < MAX_MATERIAL_COUNT);
    
    //Create Shaders
    game_state->shaders[SHADER_TEXTURE] = shader_create_program("./shaders/shader.vert", "./shaders/shader_light.frag");
    game_state->shaders[SHADER_COLOR] = shader_create_program("./shaders/shader.vert", "./shaders/shader_color.frag");
    game_state->shaders[SHADER_TERRAIN] = shader_create_program("./shaders/shader_terrain.vert", "./shaders/shader_terrain.frag");

    //Init textures
    game_state->game_textures[TEXTURE_TERRAIN] = texture_create("./res/grass.bmp");
    game_state->game_textures[TEXTURE_SPECULAR] = texture_create("./res/specular.bmp");
    game_state->game_textures[TEXTURE_FLOWER] = texture_create("./res/terrain/grass_flowers.bmp");
    game_state->game_textures[TEXTURE_MUD] = texture_create("./res/terrain/mud.bmp");
    game_state->game_textures[TEXTURE_PATH] = texture_create("./res/terrain/path.bmp");
    game_state->game_textures[TEXTURE_BLEND_MAP] = texture_create("./res/terrain/blend_map.bmp");
    game_state->game_textures[TEXTURE_TREE] = texture_create("./res/models/tree/tree.bmp");
    game_state->game_textures[TEXTURE_GRASS] = texture_create("./res/models/grass/grass.bmp");
    game_state->game_textures[TEXTURE_FERN] = texture_create("./res/models/grass/fern.bmp");
  
    //Init materials 
    game_state->game_materials[MATERIAL_TERRAIN] = material_create(TEXTURE_TERRAIN, TEXTURE_SPECULAR, 32);
    game_state->game_materials[MATERIAL_TREE] = material_create(TEXTURE_TREE, TEXTURE_SPECULAR, 32);
    game_state->game_materials[MATERIAL_GRASS] = material_create(TEXTURE_GRASS, TEXTURE_SPECULAR, 32);
    game_state->game_materials[MATERIAL_FERN] = material_create(TEXTURE_FERN, TEXTURE_SPECULAR, 32);
    
    //Init Meshes
    game_state->game_meshes[MESH_TREE] = mesh_load_from_obj("./res/models/tree/tree.obj", game_state);
    game_state->game_meshes[MESH_GRASS] = mesh_load_from_obj("./res/models/grass/grass.obj", game_state);
    game_state->game_meshes[MESH_FERN] = mesh_load_from_obj("./res/models/grass/fern.obj", game_state);
    game_state->game_meshes[MESH_TERRAIN] = terrain_generate();
    
    //Init MultiTexture
    MultiTexture terrain_texture = {TEXTURE_MUD, TEXTURE_FLOWER, TEXTURE_PATH, TEXTURE_BLEND_MAP};
    //Init Terrain
    game_state->terrain = terrain_create(MESH_TERRAIN, MATERIAL_TERRAIN, terrain_texture);

    //Init renderable
    //NOTE(tomi):Set static seed
    srand(123456);
    for(uint32_t i = 0; i < TREE_COUNT; ++i)
    {
        game_state->ren_tree[i] = renderable_create(MESH_TREE, MATERIAL_TREE); 
        float rand_x = (float)rand_int(-TERRAIN_SIZE/2, TERRAIN_SIZE/2);
        float rand_z = (float)rand_int(-TERRAIN_SIZE/2, TERRAIN_SIZE/2);
        game_state->ren_tree[i].pos = new_v3(rand_x, 0, rand_z);
        game_state->ren_tree[i].scale = new_v3(4, 4, 4);
    }
    for(uint32_t i = 0; i < GRASS_COUNT; ++i)
    {
        game_state->ren_grass[i] = renderable_create(MESH_GRASS, MATERIAL_GRASS); 
        float rand_x = (float)rand_int(-TERRAIN_SIZE/2, TERRAIN_SIZE/2);
        float rand_z = (float)rand_int(-TERRAIN_SIZE/2, TERRAIN_SIZE/2);
        game_state->ren_grass[i].pos = new_v3(rand_x, 0, rand_z);
        game_state->ren_grass[i].scale = new_v3(1, 1, 1);
        game_state->ren_grass[i].has_alpha = true;
        game_state->ren_grass[i].fake_light = true;
    }
    for(uint32_t i = 0; i < FERN_COUNT; ++i)
    {
        game_state->ren_fern[i] = renderable_create(MESH_FERN, MATERIAL_FERN); 
        float rand_x = (float)rand_int(-TERRAIN_SIZE/2, TERRAIN_SIZE/2);
        float rand_z = (float)rand_int(-TERRAIN_SIZE/2, TERRAIN_SIZE/2);
        game_state->ren_fern[i].pos = new_v3(rand_x, 0, rand_z);
        game_state->ren_fern[i].scale = new_v3(.5, .5, .5);
        game_state->ren_fern[i].has_alpha = true;
        game_state->ren_fern[i].fake_light = true;
    }
    game_state->ren_terrain = renderable_create(MESH_TERRAIN, MATERIAL_TERRAIN); 
    game_state->ren_terrain.pos = new_v3(0, 0, 0);
    
    //TODO(tomi):Take out game_state from this fucntion 
    //Init Renderer queue
    renderer_add(&game_state->renderer, game_state->ren_tree, TREE_COUNT, SHADER_TEXTURE, game_state);
    renderer_add(&game_state->renderer, &game_state->ren_terrain, 1, SHADER_TERRAIN, game_state);
    renderer_add(&game_state->renderer, game_state->ren_grass, GRASS_COUNT, SHADER_TEXTURE, game_state);
    renderer_add(&game_state->renderer, game_state->ren_fern, FERN_COUNT, SHADER_TEXTURE, game_state);
    renderer_add(&game_state->renderer, &game_state->terrain, 1, SHADER_TERRAIN, game_state);

    //Init camera
    game_state->camera = {};
    game_state->camera.speed = 15;
    game_state->camera.sensibility = 0.5f;
    
    //Init Lights
    game_state->light.direction = new_v3(0.0f, -0.6f, -0.4f);
    game_state->light.ambient = new_v3(0.1f, 0.1f, 0.02f);
    game_state->light.diffuse = new_v3(0.5f, 0.5f, 0.1f);
    game_state->light.specular = new_v3(0.6f, 0.1f, 0.0f);
    
    game_state->light_terrain.direction = new_v3(0.0f, -0.6f, -0.4f);
    game_state->light_terrain.ambient = new_v3(0.1f, 0.1f, 0.1f);
    game_state->light_terrain.diffuse = new_v3(0.5f, 0.5f, 0.5f);
    game_state->light_terrain.specular = new_v3(0.0f, 0.0f, 0.0f);
    
    //TODO(tomi):Maybe create a function to init all shaders 
    //Init Shaders 
    shader_use_program(game_state->shaders[SHADER_TEXTURE]);
    shader_set_int(game_state->shaders[SHADER_TEXTURE], "material.diffuse", 0);
    shader_set_int(game_state->shaders[SHADER_TEXTURE], "material.specular", 1);
    shader_set_dir_light(game_state->shaders[SHADER_TEXTURE], "dir_light", game_state->light);
    
    shader_use_program(game_state->shaders[SHADER_TERRAIN]);
    shader_set_int(game_state->shaders[SHADER_TERRAIN], "material.diffuse", 0);
    shader_set_int(game_state->shaders[SHADER_TERRAIN], "material.specular", 1);
    shader_set_int(game_state->shaders[SHADER_TERRAIN], "r_texture", 2);
    shader_set_int(game_state->shaders[SHADER_TERRAIN], "g_texture", 3);
    shader_set_int(game_state->shaders[SHADER_TERRAIN], "b_texture", 4);
    shader_set_int(game_state->shaders[SHADER_TERRAIN], "blend_texture", 5);
    shader_set_dir_light(game_state->shaders[SHADER_TERRAIN], "dir_light", game_state->light_terrain);
    
    m4 p = perspective(60.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 1000.0f);
    for(int i = 0; i < SHADER_COUNT; ++i)
    { 
        shader_use_program(game_state->shaders[i]);
        shader_set_m4(game_state->shaders[i], "projection", p);
        shader_set_v3(game_state->shaders[i], "sky_color", game_state->sky_color);
    }
}

void game_update(GameState* game_state, float dt)
{
    //Move camera
    if(game_state->keys[SDL_SCANCODE_W])
    {
        camera_move_forward(&game_state->camera, dt);
    }
    if(game_state->keys[SDL_SCANCODE_S])
    {
        camera_move_backward(&game_state->camera, dt);
    }
    if(game_state->keys[SDL_SCANCODE_A])
    {
        camera_move_left(&game_state->camera, dt);
    }
    if(game_state->keys[SDL_SCANCODE_D])
    {
        camera_move_right(&game_state->camera, dt);
    }
    if(game_state->keys[SDL_SCANCODE_LSHIFT])
    {
        camera_move_up(&game_state->camera, dt);
    }
    if(game_state->keys[SDL_SCANCODE_LCTRL])
    {
        camera_move_down(&game_state->camera, dt);
    }
  
    
    camera_set_direction(&game_state->camera, game_state->mouse_offset_x, game_state->mouse_offset_y);
    game_state->mouse_offset_x = 0;
    game_state->mouse_offset_y = 0;
   
    //TODO(tomi):Maybe create a function to update all shaders 
    for(int i = 0; i < SHADER_COUNT; ++i)
    {
        shader_use_program(game_state->shaders[i]);
        shader_set_v3(game_state->shaders[i], "view_pos", game_state->camera.pos);
        camera_update(&game_state->camera, game_state->shaders[i]);
    }
}

void game_render(GameState* game_state)
{
    glClearColor(game_state->sky_color.x, game_state->sky_color.y, game_state->sky_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderer_render(&game_state->renderer, game_state);

    SDL_GL_SwapWindow(game_state->window);
}

int main(int argc, char* argv[])
{
    GameState* game_state = (GameState*)malloc(sizeof(GameState));
    game_init(game_state);

    uint32_t last_time = 0;
    float dt = 0.016;
    uint32_t current_time;
    
    while(game_state->running)
    {
        process_input(game_state);
        
        game_update(game_state, dt);
        game_render(game_state);
        
        current_time = SDL_GetTicks();
        dt = ((float)current_time - (float)last_time) / 1000.0f;
        last_time = current_time;
    
    }

	return 0;
}
