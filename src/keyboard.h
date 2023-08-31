#ifndef PROGRESSIVE_KEYBOARD_H
#define PROGRESSIVE_KEYBOARD_H

#include <GLFW/glfw3.h>

#include "global_state.h"
#include "types.h"

typedef void (*KeyListener)(GlobalState *state, int key, int scancode, int action, int mods);

void keyboard_on_key(GLFWwindow *window, int key, int scancode, int action, int mods);

bool keyboard_is_key_pressed(int key);

bool keyboard_add_key_listener(KeyListener);

bool keyboard_remove_key_listener(KeyListener);

#endif //PROGRESSIVE_KEYBOARD_H
