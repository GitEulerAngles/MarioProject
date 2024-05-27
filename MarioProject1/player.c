#include "player.h"

struct playerAnimation createAnimation() {
    struct playerAnimation newP;
    newP.type = IDLE;
    newP.frame = 0;
    newP.delay = 16;
    return newP;
}

float transition(float x) {
    float normal = (x - 2) / (170);

    return 0.5 * pow(20, normal);
}

void updateCamera(struct Vector2f pos) {
    if (pos.x - cameraPos.x < 358) {
        cameraPos.x = pos.x - 358;
    }
    else if (pos.x - cameraPos.x > 558) {
        cameraPos.x = pos.x - 558;
    }

    if (pos.y - cameraPos.y < 357) {
        cameraPos.y = pos.y - 357;
    }
    else if (pos.y - cameraPos.y > 557) {
        cameraPos.y = pos.y - 557;
    }

    if (grounded && pos.y - cameraPos.y < 557) {
        cameraPos.y -= transition(557 - (pos.y - cameraPos.y));
    }
}
