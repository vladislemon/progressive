#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "camera.h"
#include "global_state.h"
#include "keyboard.h"
#include "mesh.h"
#include "mesh_builder.h"
#include "mouse.h"
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

static void updateCamera(GlobalState *state) {
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
        if (state->cursorPosInitialized) {
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
        camera_rotate(&state->camera, rotatePitch, rotateYaw);
        prevCursorX = cursorX;
        prevCursorY = cursorY;
    }
    float move = (float) (moveSpeed * deltaTime);
    if (keyboard_is_key_pressed(GLFW_KEY_LEFT_CONTROL)) {
        move *= 4;
    }
    if (keyboard_is_key_pressed(GLFW_KEY_W)) {
        camera_move_forward(&state->camera, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_S)) {
        camera_move_backward(&state->camera, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_A)) {
        camera_move_left(&state->camera, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_D)) {
        camera_move_right(&state->camera, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_SPACE)) {
        camera_move_up(&state->camera, move);
    }
    if (keyboard_is_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
        camera_move_down(&state->camera, move);
    }
    prevTime = time;
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
        updateCamera(state);
        camera_get_view(&state->camera, &view);
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
            state->camera.position[0], state->camera.position[1], state->camera.position[2],
            state->camera.pitch / GLM_PIf, state->camera.yaw / GLM_PIf);
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
    state.camera = camera_create();
    camera_set_position(&state.camera, 0, 0, -2);
    camera_set_rotation(&state.camera, 0, 0);
    state.world = world_create();
    world_setBlock(state.world, 0, 0, 0, 1);
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
    float *vertices;
    usize meshSize;
    mesh_builder_build(state.world, &vertices, &meshSize);
    mesh_update(&state.mesh, (isize) meshSize, vertices);
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
