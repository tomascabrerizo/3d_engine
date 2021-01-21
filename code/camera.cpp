#include "camera.h"
#include "shader.h"

Camera camera_create(v3 pos, v3 front, v3 w_up)
{
    Camera c = {};
    c.pos = pos;
    c.front = front;
    c.world_up = w_up;
    return c;
}

void camera_update(Camera* camera, uint32_t shader)
{
    shader_use_program(shader);
    m4 v = look_at(camera->pos, camera->pos+camera->front, camera->world_up);
    shader_set_m4(shader, "view", v);
}

void camera_set_direction(Camera* camera, float offset_x, float offset_y)
{
    float sen = 0.6f;
    camera->yaw += offset_x * sen;
    camera->pitch -= offset_y * sen;
    if(camera->pitch > 89.0f)
        camera->pitch =  89.0f;
    if(camera->pitch < -89.0f)
        camera->pitch = -89.0f;
    v3 camera_direction= {};
    camera_direction.x = cosf(to_rad(camera->yaw)) * cosf(to_rad(camera->pitch));
    camera_direction.y = sinf(to_rad(camera->pitch));
    camera_direction.z = sinf(to_rad(camera->yaw)) * cosf(to_rad(camera->pitch));
    camera->front = normalize(camera_direction);
}


void camera_move_forward(Camera* camera, float dt)
{
    v3 camera_right = normalize(cross(camera->front, camera->world_up));
    camera->pos += normalize(cross(camera->world_up, camera_right)) * camera->speed  * dt;
}

void camera_move_backward(Camera* camera, float dt)
{
    v3 camera_right = normalize(cross(camera->front, camera->world_up));
    camera->pos += normalize(cross(camera_right, camera->world_up)) * camera->speed * dt;
}

void camera_move_right(Camera* camera, float dt)
{
    v3 camera_right = normalize(cross(camera->front, camera->world_up));
    camera->pos += camera_right * camera->speed * dt;
}

void camera_move_left(Camera* camera, float dt)
{
    v3 camera_right = normalize(cross(camera->front, camera->world_up));
    camera->pos -= camera_right * camera->speed * dt;
}

void camera_move_up(Camera* camera, float dt)
{
    camera->pos += camera->world_up * camera->speed * dt;
}

void camera_move_down(Camera* camera, float dt)
{
    camera->pos -= camera->world_up * camera->speed * dt;
}
