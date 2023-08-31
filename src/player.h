#ifndef PROGRESSIVE_PLAYER_H
#define PROGRESSIVE_PLAYER_H

#include <cglm/cglm.h>

#include "camera.h"
#include "types.h"

typedef struct Player_s {
    vec3 position;
    float pitch;
    float yaw;
    bool cursorPosInitialized;
    Camera camera;
} Player;

Player player_create();

void player_update(Player *player);

#endif //PROGRESSIVE_PLAYER_H
