#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "global_state.h"
#include "keyboard.h"
#include "mesh.h"
#include "mesh_builder.h"
#include "mouse.h"
#include "player.h"
#include "shader_program.h"
#include "texture.h"
#include "thread.h"
#include "world.h"

#define PROJECT_NAME_STRING "progressive"
#define INITIAL_WINDOW_WIDTH 800
#define INITIAL_WINDOW_HEIGHT 600
#define LIMIT_FRAMERATE

static void updateViewportAndProjection(GlobalState *state, mat4 *projection) {
    static int prevWidth;
    static int prevHeight;
    if (state->framebufferWidth != prevWidth || state->framebufferHeight != prevHeight) {
        glViewport(0, 0, state->framebufferWidth, state->framebufferHeight);
        float aspect = (float) state->framebufferWidth / (float) state->framebufferHeight;
        glm_perspective(GLM_PI_4, aspect, 0.1f, 1000.0f, *projection);
        prevWidth = state->framebufferWidth;
        prevHeight = state->framebufferHeight;
    }
}

static void rebuildMesh(GlobalState *state) {
    float *vertices;
    usize meshSize;
    mesh_builder_build(state->world, &vertices, &meshSize);
    mesh_update(&state->mesh, (isize) meshSize, vertices);
    free(vertices);
}

static void *renderThreadFunc(void *arg) {
    GlobalState *state = (GlobalState *) arg;
    glfwMakeContextCurrent(state->window);
    puts(glfwGetVersionString());
#ifdef LIMIT_FRAMERATE
    if (glfwExtensionSupported("WGL_EXT_swap_control_tear") || glfwExtensionSupported("GLX_EXT_swap_control_tear")) {
        glfwSwapInterval(-1);
    } else {
        glfwSwapInterval(1);
    }
#else
    glfwSwapInterval(0);
#endif
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view;
    mat4 projection;
    mat4 mvp;
    while (!state->renderThreadShouldStop) {
        updateViewportAndProjection(state, &projection);
        player_update(&state->player);
        if (world_is_updated(state->world, true)) {
            rebuildMesh(state);
        }
        camera_get_view(&state->player.camera, &view);
        glm_mul(projection, view, mvp);
        glm_mul(mvp, model, mvp);
        glUniformMatrix4fv(0, 1, false, (const GLfloat *) mvp);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh_draw(&state->mesh);
        glfwSwapBuffers(state->window);
    }
    state->renderThreadStopped = true;
    return 0;
}

static void updateTitle(GlobalState *state) {
    char title[128];
    sprintf(title, "%s | x=%f | y=%f | z=%f | pitch=%f PI | yaw=%f PI", PROJECT_NAME_STRING,
            state->player.position[0], state->player.position[1], state->player.position[2],
            state->player.pitch / GLM_PIf, state->player.yaw / GLM_PIf);
    glfwSetWindowTitle(state->window, title);
}

static void updateTitleOnKey(GlobalState *state, int key, int scancode, int action, int mods) {
    updateTitle(state);
}

static void updateTitleOnCursor(GlobalState *state, double x, double y) {
    updateTitle(state);
}

static void onFramebufferSize(GLFWwindow *window, int width, int height) {
    GlobalState *state = glfwGetWindowUserPointer(window);
    state->framebufferWidth = width;
    state->framebufferHeight = height;
}

int main() {
    GlobalState state = {};
    state.framebufferWidth = INITIAL_WINDOW_WIDTH;
    state.framebufferHeight = INITIAL_WINDOW_HEIGHT;
    state.world = world_create();
    world_setBlock(state.world, 0, 0, 0, 1);
    state.player = player_create();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    state.window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, PROJECT_NAME_STRING, NULL, NULL);
    glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(state.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        puts("Raw mouse input enabled\n");
    }
    glfwSetWindowUserPointer(state.window, &state);
    glfwSetFramebufferSizeCallback(state.window, onFramebufferSize);
    glfwSetKeyCallback(state.window, keyboard_on_key);
    keyboard_add_key_listener(updateTitleOnKey);
    glfwSetCursorPosCallback(state.window, mouse_on_cursor_pos);
    mouse_add_cursor_pos_listener(updateTitleOnCursor);
    glfwSetMouseButtonCallback(state.window, mouse_on_button);
    glfwMakeContextCurrent(state.window);
    gladLoadGL(glfwGetProcAddress);

    //dev
    state.mesh = mesh_create();
    rebuildMesh(&state);
    Texture texture = texture_create("../textures/uvtemplate.png");
    texture_bind(&texture);
    ShaderProgram shaderProgram = shader_program_create("../shaders/shader.vert", "../shaders/shader.frag");
    shader_program_use(shaderProgram);
    //dev

    glfwMakeContextCurrent(0);
    Thread renderThread = thread_create(renderThreadFunc, &state);
    while (!glfwWindowShouldClose(state.window)) {
        glfwWaitEvents();
    }
    state.renderThreadShouldStop = true;
    while (!state.renderThreadStopped) {
        glfwWaitEvents();
    }
    thread_join(renderThread, 0);
    glfwDestroyWindow(state.window);
    glfwTerminate();
    return 0;
}
