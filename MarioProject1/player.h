#ifndef PLAYER
#define PLAYER

#include "math.h"


enum animationType { IDLE, WALKING, JUMPING, FALLING };

struct playerAnimation {
    enum animationType type;
    int16_t  frame;
    int16_t  delay;
};

struct Vector2f cameraPos;
struct Vector2f vel;

_Bool grounded;
struct playerAnimation animation;

void updateCamera(struct Vector2f pos);
struct playerAnimation createAnimation();

#endif // PLAYER
