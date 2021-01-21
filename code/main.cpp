#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "shader.h"
#include "math.h"
#include "cube.h"
#include "game_state.h"

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

void init_game_state(GameState* game_state)
{
    game_state->window = initialize_platform(game_state);
    //Shader setup
    game_state->shader_program = shader_create_program("./shaders/shader.vert", "./shaders/shader.frag");
    shader_use_program(game_state->shader_program);
    m4 p = perspective(60.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader_set_m4(game_state->shader_program, "projection", p);
    
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
    game_state->game_materials[MATERIAL_TEST].shininess = 64;
   
    //Init light 
    game_state->game_lights[LIGHT_TEST].ambient = new_v3(0.1, 0.1, 0.1);
    game_state->game_lights[LIGHT_TEST].diffuse = new_v3(0.8, 0.8, 0.8);
    game_state->game_lights[LIGHT_TEST].specular = new_v3(1, 1, 1); 

}

void game_init(GameState* game_state)
{
    init_game_state(game_state);
     
    shader_set_light(game_state->shader_program, "light", game_state->game_lights[LIGHT_TEST]);

    game_state->ren_cube = renderable_create(MESH_CUBE, TEXTURE_WOODBOX, TEXTURE_WOODBOX_SPECULAR, MATERIAL_TEST); 
    game_state->ren_cube.pos = new_v3(0, 0, 0);
    game_state->ren_cube.scale = new_v3(3, 3, 3);
    game_state->ren_cube.rotate = new_v3(0, 0, 0);
    //Lighting Test
    game_state->ren_light = renderable_create(MESH_CUBE, new_v3(1, 1, 1)); 
    game_state->ren_light.pos = new_v3(3, 2.3, 4.3);
    game_state->ren_light.scale = new_v3(0.25f, 0.25f, 0.25f);
    
    shader_set_int(game_state->shader_program, "material.diffuse", 0);
    shader_set_int(game_state->shader_program, "material.specular", 1);

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
  
    //Specular light
    shader_set_v3(game_state->shader_program, "view_pos", game_state->camera.pos);
    //Lighting Rotating in circle
    //float r = 6;
    //static float angle = 0;
    //game_state->ren_light.pos.x = r * cosf(to_rad(angle));
    //game_state->ren_light.pos.z = r * sinf(to_rad(angle));
    //game_state->ren_light.pos.y = sinf(to_rad(angle*4));
    //angle = angle >= 360 ? 0 : angle + 10 * dt;
    shader_set_v3(game_state->shader_program, "light.position", game_state->ren_light.pos);

    camera_set_direction(&game_state->camera, game_state->mouse_offset_x, game_state->mouse_offset_y);
    //TODO(tomi):See where to put this code so its makes more senses
    game_state->mouse_offset_x = 0;
    game_state->mouse_offset_y = 0;
    camera_update(&game_state->camera, game_state->shader_program);

    renderable_update(&game_state->ren_cube); 
    renderable_update(&game_state->ren_light); 
}

void game_render(GameState* game_state)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    renderable_render(game_state->ren_cube, game_state->shader_program, game_state);
    renderable_render(game_state->ren_light, game_state->shader_program, game_state);
    
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
