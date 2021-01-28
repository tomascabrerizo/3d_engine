#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "shader.h"
#include "math.h"
#include "cube.h"
#include "game_state.h"
#include "renderable.h"
#include "utils.h"

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
        printf("SDL cannot create opengl context\n");
        game_state->running = false;
    }
    GLenum glew_error = glewInit();
    if(glew_error != GLEW_OK)
    {
	    printf("Error initilizing glew: %s\n", glewGetErrorString(glew_error));
        game_state->running = false;
    }

    glEnable(GL_DEPTH_TEST);
    SDL_GL_SetSwapInterval(1);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    printf("OpenGL version: (%s)\n", glGetString(GL_VERSION));
    
    return window; 
}

void game_init(GameState* game_state)
{
    game_state->window = initialize_platform(game_state);
    //Shader setup
    game_state->shader_program = shader_create_program("./shaders/shader.vert", "./shaders/shader_light.frag");
    game_state->shader_program2 = shader_create_program("./shaders/shader.vert", "./shaders/shader_light_src.frag");
    
    m4 p = perspective(60.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader_set_m4(game_state->shader_program, "projection", p);
    shader_set_m4(game_state->shader_program2, "projection", p);
    
    //Init textures
    game_state->game_textures[TEXTURE_CUBE] = texture_create("./res/texture.bmp");
    game_state->game_textures[TEXTURE_BOX] = texture_create("./res/box.bmp");
    game_state->game_textures[TEXTURE_WOODBOX] = texture_create("./res/woodbox.bmp");
    game_state->game_textures[TEXTURE_WOODBOX_SPECULAR] = texture_create("./res/woodbox_specular.bmp");
    game_state->game_textures[TEXTURE_BACKPACK_DIFUSE] = texture_create("./res/models/backpack/diffuse.bmp");
    game_state->game_textures[TEXTURE_BACKPACK_SPECULAR] = texture_create("./res/models/backpack/specular.bmp");
  
    //Init materials 
    game_state->game_materials[MATERIAL_TEST] = material_create(TEXTURE_WOODBOX, TEXTURE_WOODBOX_SPECULAR, 32);
    game_state->game_materials[MATERIAL_BACKPACK] = material_create(TEXTURE_BACKPACK_DIFUSE, TEXTURE_BACKPACK_SPECULAR, 32);
    shader_set_int(game_state->shader_program, "material.diffuse", 0);
    shader_set_int(game_state->shader_program, "material.specular", 1);

    MeshIndex_ backpack_index = load_obj_file("./res/models/backpack/backpack.obj", game_state);
    (void)backpack_index;
    for(uint32_t i = backpack_index.f_index; i < backpack_index.f_index + backpack_index.count; ++i)
    {
        mesh_initialize(&game_state->game_meshes[i]);
    }

    //TODO(tomi): make a good way to now where the renderable meshes starts
    //Init backpack renderable
    game_state->ren_backpack = renderable_create(backpack_index.f_index, backpack_index.count, MATERIAL_BACKPACK); 

    //Init camera
    game_state->camera.speed = 3;
    game_state->camera.sensibility = 0.5f;
    
    //Init Lights
    game_state->light_backpack.direction = new_v3(-0.2f, -0.5f, -0.3f);
    game_state->light_backpack.ambient = new_v3(0.4f, 0.4f, 0.4f);
    game_state->light_backpack.diffuse = new_v3(0.6f, 0.6f, 0.6f);
    game_state->light_backpack.specular = new_v3(0.9f, 0.9f, 0.9f);
    shader_set_dir_light(game_state->shader_program, "dir_light", game_state->light_backpack);

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
  
    shader_set_v3(game_state->shader_program, "view_pos", game_state->camera.pos);
    shader_set_v3(game_state->shader_program2, "view_pos", game_state->camera.pos);
    
    camera_set_direction(&game_state->camera, game_state->mouse_offset_x, game_state->mouse_offset_y);
    game_state->mouse_offset_x = 0;
    game_state->mouse_offset_y = 0;
    camera_update(&game_state->camera, game_state->shader_program);
    camera_update(&game_state->camera, game_state->shader_program2);

    renderable_update(&game_state->ren_backpack);
}

void game_render(GameState* game_state)
{
    //TODO(tomi):Create a good renderer to render the differents vaos
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderable_render(game_state->ren_backpack, game_state->shader_program, game_state);

    SDL_GL_SwapWindow(game_state->window);
}

int main(int argc, char* argv[])
{
    GameState game_state = {};
    game_init(&game_state);

    uint32_t last_time = 0;
    float dt = 0.016;
    uint32_t current_time;

    while(game_state.running)
    {
        process_input(&game_state);
        
        game_update(&game_state, dt);
        game_render(&game_state);
        
        current_time = SDL_GetTicks();
        dt = ((float)current_time - (float)last_time) / 1000.0f;
        last_time = current_time;
    
    }

	return 0;
}
