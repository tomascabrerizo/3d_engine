#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "shader.h"
#include "math.h"
#include "cube.h"
#include "texture.h"
#include "mesh.h"
#include "renderable.h"
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

int main(int argc, char* argv[])
{
    //TODO(tomi):Create game_init function, this function can be upgrade a lot
    GameState game_state = {};
    SDL_Window* window = initialize_platform(&game_state);
    
    //Shader setup
    uint32_t shader_program = shader_create_program("./shaders/shader.vert", "./shaders/shader.frag");
    shader_use_program(shader_program);
    m4 p = perspective(60.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);
    shader_set_m4(shader_program, "projection", p);
    printf("Projection:\n");
    m4_dump(p);
    
    game_state.game_textures[TEXTURE_CUBE] = texture_create("./res/texture.bmp");
    game_state.game_textures[TEXTURE_BOX] = texture_create("./res/box.bmp");
    game_state.game_meshes[MESH_CUBE] = mesh_create(cube_vert, sizeof(cube_vert), cube_tex, sizeof(cube_tex), cube_normal, sizeof(cube_normal));
    game_state.game_materials[MATERIAL_TEST] = material_create(new_v3(0.3, 0.3, 0.3), new_v3(0.7, 0.7, 0.7), new_v3(1, 1, 1), 64.0f);
    game_state.game_lights[LIGHT_TEST] = light_create({}, new_v3(0.2, 0.2, 0.2), new_v3(0.4, 0.4, 0.4), new_v3(1, 1, 1)); 
    shader_set_light(shader_program, "light", game_state.game_lights[LIGHT_TEST]);

    Renderable ren_cube = renderable_create(MESH_CUBE, TEXTURE_BOX, MATERIAL_TEST); 
    ren_cube.pos = new_v3(0, 0, 0);
    ren_cube.scale = new_v3(3, 3, 3);
    ren_cube.rotate = new_v3(0, 0, 0);
    //Lighting Test
    Renderable ren_light = renderable_create(MESH_CUBE, new_v3(1, 1, 1)); 
    ren_light.pos = new_v3(0, 5, 0);
    ren_light.scale= new_v3(0.25f, 0.25f, 0.25f);
    shader_set_v3(shader_program, "light_color", ren_light.color); 
    
    shader_set_material(shader_program, "material", {});

    while(game_state.running)
    {
        process_input(&game_state);

        //TODO(tomi):Create game_update function
        //Move camera
        float dt = 0.016;
        if(game_state.keys[SDL_SCANCODE_W])
        {
            camera_move_forward(&game_state.camera, dt);
        }
        if(game_state.keys[SDL_SCANCODE_S])
        {
            camera_move_backward(&game_state.camera, dt);
        }
        if(game_state.keys[SDL_SCANCODE_A])
        {
            camera_move_left(&game_state.camera, dt);
        }
        if(game_state.keys[SDL_SCANCODE_D])
        {
            camera_move_right(&game_state.camera, dt);
        }
        if(game_state.keys[SDL_SCANCODE_LSHIFT])
        {
            camera_move_up(&game_state.camera, dt);
        }
        if(game_state.keys[SDL_SCANCODE_LCTRL])
        {
            camera_move_down(&game_state.camera, dt);
        }
      
        //Specular light
        shader_set_v3(shader_program, "view_pos", game_state.camera.pos);
        //Lighting Rotating in circle
        float r = 6;
        static float angle = 0;
        ren_light.pos.x = r * cosf(to_rad(angle));
        ren_light.pos.z = r * sinf(to_rad(angle));
        ren_light.pos.y = sinf(to_rad(angle*4));
        angle = angle >= 360 ? 0 : angle + 0.3;
        shader_set_v3(shader_program, "light_pos", ren_light.pos);
    
        camera_set_direction(&game_state.camera, game_state.mouse_offset_x, game_state.mouse_offset_y);
        //TODO(tomi):See where to put this code so its makes more senses
        game_state.mouse_offset_x = 0;
        game_state.mouse_offset_y = 0;
        camera_update(&game_state.camera, shader_program);

        renderable_update(&ren_cube); 
        renderable_update(&ren_light); 
        
        //TODO(tomi):Create game_render function 
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        renderable_render(ren_cube, shader_program, &game_state);
        renderable_render(ren_light, shader_program, &game_state);
        
        SDL_GL_SwapWindow(window);
    }

	return 0;
}
