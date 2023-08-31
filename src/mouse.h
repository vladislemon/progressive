#ifndef PROGRESSIVE_MOUSE_H
#define PROGRESSIVE_MOUSE_H

#include <GLFW/glfw3.h>

#include "global_state.h"
#include "types.h"

typedef void (*CursorPosListener)(GlobalState *state, double x, double y);

typedef void (*ButtonListener)(GlobalState *state, int button, int action, int mods);

void mouse_on_cursor_pos(GLFWwindow *window, double x, double y);

void mouse_on_button(GLFWwindow *window, int button, int action, int mods);

void mouse_get_cursor_pos(double *x, double *y);

double mouse_get_cursor_x();

double mouse_get_cursor_y();

bool mouse_is_button_pressed(int button);

bool mouse_add_cursor_pos_listener(CursorPosListener);

bool mouse_remove_cursor_pos_listener(CursorPosListener);

bool mouse_add_button_listener(ButtonListener);

bool mouse_remove_button_listener(ButtonListener);

#endif //PROGRESSIVE_MOUSE_H
