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
    // Bitmaps
    FIBITMAP* bitmap;
    FIBITMAP* resizedBitmap;
    // Image coordinates
    struct Vector2i imageCoordinate;
    struct Vector2i imageDimension;
    // Image dimensions
    struct Vector2i originalDim;
    struct Vector2i newDim;
    // On screen data
    struct Vector2f pos;
    struct Vector2i dim;
};

struct sprite dynamicSprites[1];
struct sprite staticSprites[100];
struct playerAnimation animation;
struct Vector2f vel;
_Bool grounded;

enum   blockType blockTypes[100];

struct playerAnimation createAnimation();

void drawPlayer(struct playerAnimation* p);
void updateBlocks();
void createSprite(struct sprite* sprites, int index, char path[]);

#endif // SPRITES
