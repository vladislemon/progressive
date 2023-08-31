#include "keyboard.h"

#define LISTENER_MAX_COUNT 64

static bool keys[GLFW_KEY_LAST];
static KeyListener listeners[LISTENER_MAX_COUNT];

void keyboard_on_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
    GlobalState *state = glfwGetWindowUserPointer(window);
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (listeners[i] != 0) {
            listeners[i](state, key, scancode, action, mods);
        }
    }
}

bool keyboard_is_key_pressed(int key) {
    return keys[key];
}

bool keyboard_add_key_listener(KeyListener listener) {
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (listeners[i] == 0) {
            listeners[i] = listener;
            return true;
        }
    }
    return false;
}

bool keyboard_remove_key_listener(KeyListener listener) {
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (listeners[i] == listener) {
            listeners[i] = 0;
            return true;
        }
    }
    return false;
}
