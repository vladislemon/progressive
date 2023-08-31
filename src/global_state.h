#ifndef PROGRESSIVE_GLOBAL_STATE_H
#define PROGRESSIVE_GLOBAL_STATE_H

#include <GLFW/glfw3.h>

#include "camera.h"
#include "mesh.h"
#include "player.h"
#include "types.h"
#include "world.h"

typedef struct GlobalState_s {
    GLFWwindow *window;
    int framebufferWidth;
    int framebufferHeight;
    bool renderThreadShouldStop;
    bool renderThreadStopped;
    World world;
    Mesh mesh;
    Player player;
} GlobalState;

#endif //PROGRESSIVE_GLOBAL_STATE_H
