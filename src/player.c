#include "player.h"

#include <GLFW/glfw3.h>

#include "global_state.h"
#include "keyboard.h"
#include "mouse.h"
#include "world.h"

#define WORLD_UP ((vec3){0,1,0})

static void onKey(GlobalState *state, int key, int scancode, int action, int mods) {
}

static void onCursorPos(GlobalState *state, double x, double y) {
    state->player.cursorPosInitialized = true;
}

static void onMouseButton(GlobalState *state, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2) {
            i32 x = (i32) state->player.position[0];
            i32 y = (i32) state->player.position[1];
            i32 z = (i32) state->player.position[2];
            i8 block = button == GLFW_MOUSE_BUTTON_2 ? 1 : 0;
            world_setBlock(state->world, x, y, z, block);
        }
    }
}

Player player_create() {
    Player player = {};
    player.camera = camera_create();
    keyboard_add_key_listener(onKey);
    mouse_add_cursor_pos_listener(onCursorPos);
    mouse_add_button_listener(onMouseButton);
    return player;
}

void player_update(Player *player) {
    static bool first = true;
    static bool cursorFirst = true;
    static double prevTime;
    static double prevCursorX, prevCursorY;
    double cursorX = mouse_get_cursor_x();
    double cursorY = mouse_get_cursor_y();
    if (first) {
        prevTime = glfwGetTime();
        first = false;
    }
    if (cursorFirst) {
        if (player->cursorPosInitialized) {
            prevCursorX = cursorX;
            prevCursorY = cursorY;
            cursorFirst = false;
        }
    }
    double moveSpeed = 5;
    double rotateSpeed = 0.001;
    double time = glfwGetTime();
    double deltaTime = time - prevTime;
    if (!cursorFirst) {
        double deltaX = cursorX - prevCursorX;
        double deltaY = cursorY - prevCursorY;
        float rotatePitch = (float) (-deltaY * rotateSpeed);
        float rotateYaw = (float) (-deltaX * rotateSpeed);
        player->pitch += rotatePitch;
        player->yaw += rotateYaw;
        camera_set_rotation(&player->camera, player->pitch, player->yaw);
        prevCursorX = cursorX;
        prevCursorY = cursorY;
    }
    vec3 move = GLM_VEC3_ZERO_INIT;
    //float move = (float) (moveSpeed * deltaTime);
    if (keyboard_is_key_pressed(GLFW_KEY_LEFT_CONTROL)) {
        moveSpeed *= 4;
    }
    if (keyboard_is_key_pressed(GLFW_KEY_W)) {
        glm_vec3_muladds(player->camera.directionAtPlane, 1, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_S)) {
        glm_vec3_muladds(player->camera.directionAtPlane, -1, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_A)) {
        glm_vec3_muladds(player->camera.right, -1, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_D)) {
        glm_vec3_muladds(player->camera.right, 1, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_SPACE)) {
        glm_vec3_muladds(WORLD_UP, 1, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
        glm_vec3_muladds(WORLD_UP, -1, move);
    }
    glm_normalize(move);
    glm_vec3_muladds(move, (float) (moveSpeed * deltaTime), player->position);
    camera_set_position(&player->camera, player->position[0], player->position[1], player->position[2]);
    prevTime = time;
}
