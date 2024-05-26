#ifndef SPRITES
#define SPRITES

#include "math.h"
#define MULTIPLIER 10

enum animationType {IDLE, WALKING, JUMPING, FALLING};

enum blockType {AIR, QUESTION, GRASS};

struct playerAnimation {
    enum animationType type;
    int frame;
    int delay;
};

struct sprite {
    // Image coordinates
    struct Vector2i imageCoordinate;
    struct Vector2i imageDimension;
    // On screen data
    struct Vector2f pos;
    struct Vector2i dim;
};

FIBITMAP* marioTexture;
FIBITMAP* blockTexture;

struct sprite dynamicSprites[1];
struct sprite staticSprites[100];
struct playerAnimation animation;
struct Vector2f vel;
_Bool grounded;

enum   blockType blockTypes[100];

struct playerAnimation createAnimation();

void drawPlayer(struct playerAnimation* p);
void updateBlocks();
FIBITMAP* createSprite(char path[]);

#endif // SPRITES
