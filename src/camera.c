#include "camera.h"
#include <cglm/cglm.h>
#include <math.h>

#define WORLD_UP ((vec3){0,1,0})

static void update_rotation(Camera *camera) {
    if (camera->pitch < -GLM_PI_2f) {
        camera->pitch = -GLM_PI_2f;
    } else if (camera->pitch > GLM_PI_2f) {
        camera->pitch = GLM_PI_2f;
    }
    while (camera->yaw > 2 * GLM_PIf) {
        camera->yaw -= 2 * GLM_PIf;
    }
    while (camera->yaw < 0) {
        camera->yaw += 2 * GLM_PIf;
    }
    camera->direction[0] = cosf(camera->pitch) * sinf(camera->yaw);
    camera->direction[1] = sinf(camera->pitch);
    camera->direction[2] = cosf(camera->pitch) * cosf(camera->yaw);
    camera->directionAtPlane[0] = sinf(camera->yaw);
    camera->directionAtPlane[1] = 0;
    camera->directionAtPlane[2] = cosf(camera->yaw);
    camera->right[0] = sinf(camera->yaw - GLM_PI_2f);
    camera->right[1] = 0;
    camera->right[2] = cosf(camera->yaw - GLM_PI_2f);
    glm_cross(camera->right, camera->direction, camera->up);
}

Camera camera_create() {
    Camera camera;
    glm_vec3_zero(camera.position);
    camera.pitch = 0;
    camera.yaw = 0;
    camera_rotate(&camera, 0, 0);
    return camera;
}

void camera_get_view(Camera *camera, mat4 *view) {
    glm_look(camera->position, camera->direction, camera->up, view[0]);
}

void camera_set_position(Camera *camera, float x, float y, float z) {
    camera->position[0] = x;
    camera->position[1] = y;
    camera->position[2] = z;
}

void camera_set_rotation(Camera *camera, float pitch, float yaw) {
    camera->pitch = pitch;
    camera->yaw = yaw;
    update_rotation(camera);
}

void camera_move_forward(Camera *camera, float amount) {
    glm_vec3_muladds(camera->directionAtPlane, amount, camera->position);
}

void camera_move_backward(Camera *camera, float amount) {
    glm_vec3_muladds(camera->directionAtPlane, -amount, camera->position);
}

void camera_move_left(Camera *camera, float amount) {
    glm_vec3_muladds(camera->right, -amount, camera->position);
}

void camera_move_right(Camera *camera, float amount) {
    glm_vec3_muladds(camera->right, amount, camera->position);
}

void camera_move_up(Camera *camera, float amount) {
    glm_vec3_muladds(WORLD_UP, amount, camera->position);
}

void camera_move_down(Camera *camera, float amount) {
    glm_vec3_muladds(WORLD_UP, -amount, camera->position);
}

void camera_rotate(Camera *camera, float pitch, float yaw) {
    camera->pitch += pitch;
    camera->yaw += yaw;
    update_rotation(camera);
}
