#include "mouse.h"

#define LISTENER_MAX_COUNT 64

static CursorPosListener cursorPosListeners[LISTENER_MAX_COUNT];
static ButtonListener buttonListeners[LISTENER_MAX_COUNT];
static double cursorX, cursorY;
static bool buttons[GLFW_MOUSE_BUTTON_LAST];

void mouse_on_cursor_pos(GLFWwindow *window, double x, double y) {
    cursorX = x;
    cursorY = y;
    GlobalState *state = glfwGetWindowUserPointer(window);
    state->cursorPosInitialized = true;
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (cursorPosListeners[i] != 0) {
            cursorPosListeners[i](state, x, y);
        }
    }
}

void mouse_on_button(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        buttons[button] = true;
    } else if (action == GLFW_RELEASE) {
        buttons[button] = false;
    }
    GlobalState *state = glfwGetWindowUserPointer(window);
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (buttonListeners[i] != 0) {
            buttonListeners[i](state, button, action, mods);
        }
    }
}

void mouse_get_cursor_pos(double *x, double *y) {
    *x = cursorX;
    *y = cursorY;
}

double mouse_get_cursor_x() {
    return cursorX;
}

double mouse_get_cursor_y() {
    return cursorY;
}

bool mouse_is_button_pressed(int button) {
    return buttons[button];
}

bool mouse_add_cursor_pos_listener(CursorPosListener listener) {
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (cursorPosListeners[i] == 0) {
            cursorPosListeners[i] = listener;
            return true;
        }
    }
    return false;
}

bool mouse_remove_cursor_pos_listener(CursorPosListener listener) {
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (cursorPosListeners[i] == listener) {
            cursorPosListeners[i] = 0;
            return true;
        }
    }
    return false;
}

bool mouse_add_button_listener(ButtonListener listener) {
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (buttonListeners[i] == 0) {
            buttonListeners[i] = listener;
            return true;
        }
    }
    return false;
}

bool mouse_remove_button_listener(ButtonListener listener) {
    for (int i = 0; i < LISTENER_MAX_COUNT; i++) {
        if (buttonListeners[i] == listener) {
            buttonListeners[i] = 0;
            return true;
        }
    }
    return false;
}
