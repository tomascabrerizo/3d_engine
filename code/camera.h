#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
#include "math.h"

//TODO(tomi):Create a constructor function to initialize camera
struct Camera {
    float yaw = -90.0f;
    float pitch = 0.0f;
    v3 pos = new_v3(0, 1, 6);
    v3 front = new_v3(0, 0, -1);
    v3 world_up = new_v3(0, 1, 0);
    float speed = 5;
    float sensibility = 0.6f; 
};

Camera camera_create(v3 pos, v3 front, v3 w_up);
void camera_update(Camera* camera, uint32_t shader);
void camera_set_direction(Camera* camera, float offset_x, float offset_y);

void camera_move_forward(Camera* camera , float dt);
void camera_move_backward(Camera* camera, float dt);
void camera_move_right(Camera* camera, float dt);
void camera_move_left(Camera* camera, float dt);
void camera_move_up(Camera* camera, float dt);
void camera_move_down(Camera* camera, float dt);

#endif
