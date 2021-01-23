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
	);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
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
    //IMPORTANT TODO(tomi)Make differents types of shaders for the game
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
   
    //Init meshes 
    game_state->game_meshes[MESH_CUBE] = mesh_create(
            cube_vert, sizeof(cube_vert), 
            cube_tex, sizeof(cube_tex), 
            cube_normal, sizeof(cube_normal));
   
    //Init materials 
    game_state->game_materials[MATERIAL_TEST] = material_create(TEXTURE_WOODBOX, TEXTURE_WOODBOX_SPECULAR, 32);
    shader_set_int(game_state->shader_program, "material.diffuse", 0);
    shader_set_int(game_state->shader_program, "material.specular", 1);
   
    //Init light 
    game_state->dir_light.direction = new_v3(-0.2f, -1.0f, -0.3f);
    game_state->dir_light.ambient = new_v3(0.1f, 0.1f, 0.1f);
    game_state->dir_light.diffuse = new_v3(0.2f, 0.2f, 0.2f);
    game_state->dir_light.specular = new_v3(0.5f, 0.5f, 0.5f);
    
    v3 pointLightPositions[] = {
        new_v3( 0.7f,  0.2f,  2.0f),
        new_v3( 2.3f, -3.3f, -4.0f),
        new_v3(-4.0f,  2.0f, -12.0f),
        new_v3( 0.0f,  0.0f, -3.0f)
    };

    for(uint32_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        game_state->point_lights[i].position = pointLightPositions[i];
        game_state->point_lights[i].ambient = new_v3(0.05f, 0.05f, 0.05f);
        game_state->point_lights[i].diffuse = new_v3(0.8f, (0.8f), 0.8f);
        game_state->point_lights[i].specular = new_v3(1.0f, 1.0f, 1.0f);
        game_state->point_lights[i].constant = 1.0f;
        game_state->point_lights[i].linear = 0.22f; 
        game_state->point_lights[i].quadratic = 0.20f; 
    }
    
    //Init Ren_Cubes
    for(uint32_t i = 0; i < array_count(cubePositions); ++i)
    {
        float angle = 20 * i;
        game_state->ren_cubes[i] = renderable_create(MESH_CUBE, MATERIAL_TEST); 
        game_state->ren_cubes[i].pos = cubePositions[i];
        game_state->ren_cubes[i].scale = new_v3(1, 1, 1);
        game_state->ren_cubes[i].rotate = new_v3(angle, angle*3, angle*5);
    }

    //Init Ren_Light
    for(uint32_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        game_state->ren_lights[i] = renderable_create(MESH_CUBE, new_v3(1, 1, 1));
        game_state->ren_lights[i].scale = new_v3(0.3, 0.3, 0.3);
        game_state->ren_lights[i].pos = pointLightPositions[i]; 
    } 
    
    //Init camera
    game_state->camera.speed = 3;
    game_state->camera.sensibility = 0.5f;

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
    
    //Update light uniforms
    shader_set_int(game_state->shader_program, "NR_POINT_LIGHTS", (int)MAX_POINT_LIGHTS);
    shader_set_dir_light(game_state->shader_program, "dir_light", game_state->dir_light);
    for(uint32_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        char buffer[64];
        sprintf(buffer, "point_lights[%d]", i);
        shader_set_point_light(game_state->shader_program, buffer, game_state->point_lights[i]);
    }

    for(uint32_t i = 0; i < array_count(cubePositions); ++i)
    {
        renderable_update(&game_state->ren_cubes[i]);
    }
    
    for(uint32_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        renderable_update(&game_state->ren_lights[i]);
    } 
}

void game_render(GameState* game_state)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    for(uint32_t i = 0; i < array_count(cubePositions); ++i)
    {
        renderable_render(game_state->ren_cubes[i], game_state->shader_program, game_state);
    }
    for(uint32_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        renderable_render(game_state->ren_lights[i], game_state->shader_program2, game_state);
    } 

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
