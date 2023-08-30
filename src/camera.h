#ifndef PROGRESSIVE_CAMERA_H
#define PROGRESSIVE_CAMERA_H

#include <cglm/cglm.h>

typedef struct Camera_s {
    vec3 position;
    vec3 direction;
    vec3 directionAtPlane;
    vec3 right;
    vec3 up;
    float pitch;
    float yaw;
} Camera;

Camera camera_create();

void camera_get_view(Camera *camera, mat4 *view);

void camera_set_position(Camera *camera, float x, float y, float z);

void camera_set_rotation(Camera *camera, float pitch, float yaw);

void camera_move_forward(Camera *camera, float amount);

void camera_move_backward(Camera *camera, float amount);

void camera_move_left(Camera *camera, float amount);

void camera_move_right(Camera *camera, float amount);

void camera_move_up(Camera *camera, float amount);

void camera_move_down(Camera *camera, float amount);

void camera_rotate(Camera *camera, float pitch, float yaw);

#endif //PROGRESSIVE_CAMERA_H
