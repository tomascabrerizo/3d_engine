#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "math.h"
#include "texture.h"
#include "mesh.h"
#include "camera.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

#define MAX_TEXTURE_COUT 256
#define MAX_MESHES_COUNT 32

//Game textures
enum TextureIndex {
    TEXTURE_EMPTY = 0,
    TEXTURE_CUBE,
    TEXTURE_BOX,

    TEXTURE_COUNT,
};

enum MeshesIndex {
    MESH_EMPTY = 0,
    MESH_CUBE,

    MESH_COUNT
};

struct GameState {

    bool running = true;

    //TODO(tomi): Check the actual number of keys scancodes/keysym when libsdl.org works!
    //Keyboar input
    bool keys[1024];
    //Mouse 
    float mouse_offset_x = 0;
    float mouse_offset_y = 0;
    
    Camera camera;

    Texture game_textures[MAX_TEXTURE_COUT];
    Mesh game_meshes[MAX_MESHES_COUNT];

};

#endif
