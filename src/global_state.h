#ifndef PROGRESSIVE_GLOBAL_STATE_H
#define PROGRESSIVE_GLOBAL_STATE_H

#include <GLFW/glfw3.h>

#include "camera.h"
#include "mesh.h"
#include "types.h"
#include "world.h"

typedef struct GlobalState_s {
    GLFWwindow *window;
    int framebufferWidth;
    int framebufferHeight;
    bool renderThreadShouldStop;
    bool renderThreadStopped;
    bool cursorPosInitialized;
    Camera camera;
    World world;
    Mesh mesh;
} GlobalState;

#endif //PROGRESSIVE_GLOBAL_STATE_H
